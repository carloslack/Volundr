/**
 *    @file  parse.c
 *   @brief  Parse general ELF information.
 *
 * TODO: add "@param x" to doxygen comments
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#include "parse.h"
#include "write.h"
#include "validate.h"
#include "log.h"
#include "map.h"

/**
 * @defgroup parse Parse
 * @ingroup elf
 * This set of functions provides mechanisms to retrieve information from an
 * ELF image.
 */

/************ INTERFACES ************/
/**
 * @defgroup interfaces
 * @ingroup parse
 * @addtogroup interfaces Interfaces
 * @{
 */

/**
 * Helper
 * Lookup the Section Headers and return the one
 * matching name.
 */
static elf_shdr_t *_elf_get_shdr_byname(const elf_t *elfo, const sbyte *name)
{
    ASSERT_ARG_RET_NULL(elfo && name != NULL);
    ASSERT_CON_RET_NULL(elfo->shdrs);

    elf_shdr_t *shdr = NULL;
    size_t len = strlen(name);

    for(int i = 0; i<SHNUM(elfo) && !shdr; i++) {
        char *curr = elf_get_section_header_name(elfo->shdrs[i]);
        if(curr && strlen(curr) == len && !strncmp(name, curr, len))
            shdr = elfo->shdrs[i];
    }
    return shdr;
}

/**
 * @brief Read ELF Header
 *
 * Contract:
 * @note This function depends on populating (mmap) elf_t's raw image element
 * (elf_t->mapaddr). Currently the only possibility is reading ELF from
 * a disk file (by calling @ref elf_parse_file).
 * @see elf_parse_file
 *
 * @return A pointer to the ELF Header.
 */
elf_ehdr_t* elf_parse_ehdr(const elf_t *elfo)
{
    ASSERT_ARG_RET_NULL(elfo);
    ASSERT_CON_RET_NULL(elfo->mapaddr);
    return (elf_ehdr_t*)elfo->mapaddr;
}

/**
 * Contract:
 * @note This method depends on both populating the ELF object with image's
 * raw data and calling elf_parse_ehdr.
 * @see elf_parse_ehdr
 *
 * @return An array referencing all Program Headers in ELF image.
 * @note The array itself is unique for each function call, but its elements
 * are references to the ELF raw image in @ref elf_t interface. Never try
 * freeing these elements.
 *
 *
 * Parse program headers
 *  Return an array referencing all Program Headers in ELF image
 */
elf_phdr_t** elf_parse_phdrs(const elf_t *elfo)
{
    ASSERT_ARG_RET_NULL(elfo);
    ASSERT_CON_RET_NULL(elfo->mapaddr && elfo->ehdr);

#ifdef PARANOID_CHECK
    // check for inconsistency
    size_t elf_size = (size_t) elfo->ehdr->e_phentsize;
    size_t vol_size = sizeof(elf_phdr_t);

    if(vol_size != elf_size) {
        log_error("Invalid program header size : "
                "rcvd: %d xpctd: %d", elf_size, vol_size);
    }
#endif
    int i;
    elf_phdr_t **array = smalloc((PHNUM(elfo)+1) * sizeof(elf_phdr_t*));
    elf_phdr_t *ptr = (elf_phdr_t*)(elfo->ehdr->e_phoff +
            (unsigned char*)elfo->mapaddr);

    for(i=0; i<PHNUM(elfo); i++) { array[i] = ptr++; }
    array[i] = (elf_phdr_t*)0;

    return array;
}

/**
 * @brief Read Section Headers
 *
 * Contract:
 * @note This method depends on calling elf_parse_ehdr first.
 * @see elf_parse_ehdr
 *
 * @return An array with all Section Header in ELF image.
 * @note The array itself is unique for each function call, but its elements
 * are references to the ELF raw image in @ref elf_t interface. Never try
 * freeing these elements.
 */
elf_shdr_t** elf_parse_shdrs(const elf_t *elfo)
{
    ASSERT_ARG_RET_NULL(elfo);
    ASSERT_CON_RET_NULL(elfo->mapaddr && elfo->ehdr);

#ifdef PARANOID_CHECK
    // check for inconsistency
    size_t elf_size = (size_t) elfo->ehdr->e_shentsize;
    size_t vol_size = sizeof(elf_shdr_t);

    if(vol_size != elf_size)
        log_error("Invalid program header size : "
                "rcvd: %d xpctd: %d", elf_size, vol_size);
#endif
    int i;
    elf_shdr_t **array = smalloc((SHNUM(elfo)+1) * sizeof(elf_shdr_t*));
    elf_shdr_t *ptr = (elf_shdr_t*)(elfo->mapaddr + elfo->ehdr->e_shoff);

    for(i=0; i<SHNUM(elfo); i++)
        array[i] = ptr++;

    array[i] = (elf_shdr_t*)0;

    return array;
}

/**
 * This method reads an ELF file to memory and returns an interface to
 * the parsed content.
 *
 * @note Allocating the whole file to memory is possibly not the smartest of the
 * implementation, but it's the simplest for sure, and simple is good.
 *
 * @brief Creates an interface to an ELF image on disk.
 *
 * @return An interface to ELF image on disk.
 */
elf_t *elf_parse_file(const char *filename, FILE *fp)
{
    ASSERT_ARG_RET_NULL(filename && fp);
    struct mapped_file user_data;
    elf_t *elfo = smalloc(sizeof(elf_t));

    bool rc = file_read_all(&user_data, fp);
    ASSERT_ARG_RET_FALSE(rc);

    // create interface
    elfo->fsize = user_data.st.st_size;
    elfo->mapaddr = user_data.mapaddr;
    strncpy(elfo->filename, filename, sizeof(elfo->filename));

    elfo->syms_symtab = NULL;
    elfo->syms_dynsym = NULL;
    elfo->sht_symtab  = NULL;
    elfo->sht_dynsym  = NULL;

    // read ELF headers
    elfo->ehdr  = elf_parse_ehdr(elfo);
    elfo->phdrs = elf_parse_phdrs(elfo);
    elfo->shdrs = elf_parse_shdrs(elfo);

    return elfo;
}

/*@} ELF.interfaces */

/************ Headers ************/
/**
 * @defgroup headers
 * @ingroup parse
 * @addtogroup headers Headers
 * @{
 */



/** @}*/

/************ SECTIONS ************/
/**
 * @defgroup sections
 * @ingroup parse
 * @addtogroup sections Sections
 * @{
 */

/**
 * @brief Look-up Section Header index from file by its name
 *
 * Contract:
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 *
 * @return Index of given section, if any
 */
elf_word_t elf_parse_shdr_idx_byname(const sbyte *name)
{
    const elf_t *elfo = (const elf_t *)elf_get_my_elfo();
    elf_word_t idx = -1;
    if(!(elfo && elfo->shdrs && name != NULL))
        return idx;

    size_t len = strlen(name);

    for(elf_word_t i=0; i<SHNUM(elfo) && idx == -1; i++) {
        sbyte *curr = elf_get_section_header_name(elfo->shdrs[i]);
        if(strlen(curr) == len && !strncmp(name, curr, len))
            idx = i;
    }
    return idx;
}

/**
 * @brief Read Section Header Name using its index as reference
 *
 * Contract:
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 *
 * @return A string containing section's name
 */
char *elf_parse_shname_byindex(elf_word_t shidx)
{
    const elf_t *elfo = (const elf_t *)elf_get_my_elfo();
    elf_word_t n = 0;
    sbyte *shstrtab = NULL;

    ASSERT_ARG_RET_NULL(elf_validate_index(elfo, shidx));
    ASSERT_CON_RET_NULL(elfo->shdrs);

    shstrtab = elf_parse_shstrtab(NULL);
    ASSERT_RET_NULL(shstrtab && n, "Invalid .shstrtab");

    elf_word_t index = elfo->shdrs[shidx]->sh_name;
    ASSERT_RET_NULL(shidx < n, "Invalid string table index");

    return (shstrtab + index);
}

/**
 * @brief Read Section Header Name
 *
 * Contract:
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 *
 * @return A string containing section's name
 */
sbyte *elf_get_section_header_name(const elf_shdr_t *shdr)
{
    ASSERT_CON_RET_NULL(shdr);

    sbyte *shstrtab = elf_parse_shstrtab(NULL);
    return shstrtab ? SHNAME(shdr,shstrtab) : NULL;
}

/** @} */

/************ SYMBOLS ************/
/**
 * @defgroup symbols
 * @ingroup parse
 * @addtogroup symbols Symbols
 * @{
 */

/**
 * @brief Store current global symbol table.
 * @see _elf_store_symbol_table
 * @note only call this from elf_load_section_header_global_symbols
 * @see elf_load_section_header_global_symbols
 */
static void _elf_store_global_symbol_table(elf_shdr_t **shdrs,
        elf_t *elfo, elf_word_t sh_type)
{
    if (sh_type == SHT_SYMTAB)
        elfo->sht_symtab = shdrs;
    else
        elfo->sht_dynsym = shdrs;
}
/**
 * @brief Read all section headers of a given type.
 *
 * Contract:
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 *
 * @return A null terminated array of section header pointers.
 * @note The array itself is unique for each function call, but its elements
 * are references to the ELF raw image in @ref elf_t interface. Never try
 * freeing these elements.
 * @note it stores global symbol table on elfo object
 * @note stored object must be freed when no longer needed
 */
elf_shdr_t **elf_load_section_header_global_symbols(elf_word_t sh_type)
{
    elf_t *elfo = elf_get_my_elfo();
    ASSERT_ARG_RET_FALSE(sh_type == SHT_SYMTAB || sh_type == SHT_DYNSYM);
    ASSERT_CON_RET_NULL(elfo->shdrs);

    int shnum = SHNUM(elfo);
    if (!shnum) {
        log_debug("no symbols");
        return NULL;
    }

    int cnt = 0;
    elf_shdr_t **shdrs = smalloc(shnum*sizeof(elf_shdr_t*));
    for(int i=0; i<elfo->ehdr->e_shnum && elfo->shdrs[i]; i++) {
        if(elfo->shdrs[i]->sh_type == sh_type)
            shdrs[cnt++] = elfo->shdrs[i];
    }

    shdrs[cnt] = NULL;
    shdrs = srealloc(shdrs, (cnt+1)*sizeof(elf_shdr_t));

    _elf_store_global_symbol_table(shdrs, elfo, sh_type);

    return shdrs;
}

/**
 * @brief Returns pointers to all symbol tables in ELF file.
 *
 * Contract:
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 *
 * @return A null-terminated array of all symbol table section headers. Returns
 * NULL either when no symbol table is found, or when an error occurs.
 *
 * This method depends on calling elf_parse_shdrs first.
 */
elf_shdr_t **elf_get_section_header(elf_word_t sh_type)
{
    elf_shdr_t **symtabs = elf_load_section_header_global_symbols(sh_type);
    return symtabs;
}

/**
 * @brief Store current symbol table.
 *  This is useful mainly for two reason: to avoid
 *  leaking when we go out of context and to allow callee
 *  to either keep the reference for later use, although there
 *  is no free lunch the these pointers need to be freed when no
 *  longer needed.
 *  @note only call this from elf_load_section_header_symbols
 *  @see elf_load_section_header_symbols
 */
static void _elf_store_symbol_table(elf_sym_t **syms,
        elf_t *elfo, elf_word_t sh_type)
{
    if (sh_type == SHT_SYMTAB)
        elfo->syms_symtab = syms;
    else
        elfo->syms_dynsym = syms;
}
/**
 * @brief Read symbols from symbol table.
 *
 * @param symtab The Symbol Table
 *
 * @return An array of ELF Symbols. Returns NULL if there are no symbols
 */

elf_sym_t **elf_load_section_header_symbols(elf_shdr_t *symtab)
{
    ASSERT_ARG_RET_NULL(symtab);
    ASSERT_CON_RET_NULL(symtab->sh_type == SHT_SYMTAB ||
            symtab->sh_type == SHT_DYNSYM);

    int sentnum = SENTNUM(symtab);
    if (!sentnum) {
        log_debug("no symbols");
        return NULL;
    }

    elf_t *elfo = elf_get_my_elfo();
    size_t size = sentnum+1 * sizeof(elf_sym_t*);
    elf_sym_t **syms = smalloc(size * sizeof(elf_sym_t*));

    int i;
    for(i=0; i < sentnum; i++) {
        syms[i] = (elf_sym_t*)
            (symtab->sh_offset + symtab->sh_entsize*i +
             (unsigned char*)elfo->mapaddr);
    }
    syms[i] = (elf_sym_t*)0;

    /** Store the current symbol table */
    _elf_store_symbol_table(syms, elfo, symtab->sh_type);

    return syms;
}

#if 0
elf_sym_t *elf_parse_sym(const elf_shdr_t *symtab, elf_word_t index)
{
    const elf_t *elfo = (const elf_t *)elf_get_my_elfo();
    ASSERT_ARG_RET_NULL(symtab);
    ASSERT_CON_RET_NULL(elfo->mapaddr);

    if(index >= SENTNUM(symtab))
        return NULL; // symbol not found

#ifdef PARANOID_CHECK
    // TODO: validate sh_entsize and sh_off.
#endif

    return (elf_sym_t*)
        (elfo->mapaddr + symtab->sh_offset + symtab->sh_entsize*index);
}
#endif
/** @} */

/************ STRINGS ************/
/**
 * @defgroup strings
 * @ingroup parse
 * @addtogroup strings Strings
 * @{
 */

/**
 * @brief This method retrieves pointers to all string tables in ELF file.
 *
 * @return An array of pointers to all string tables in ELF file/image.
 * Returns NULL if fails. The array can be free()'d, but not its elements.
 *
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 */
#if 0
elf_shdr_t **elf_parse_all_strtabs(void)
{
    const elf_t *elfo = (const elf_t *)elf_get_my_elfo();
    elf_word_t types[] = {SHT_STRTAB};
    elf_shdr_t **strtabs = elf_load_section_header_global_symbols(types, 1);
    return strtabs;
}
#endif
/**
 * @brief Read string table pointed by section header of given index.
 *
 * Contract:
 * @note This method depends on calling elf_parse_shdrs first.
 * @see elf_parse_shdrs
 *
 * @return An array of bytes (hopefully, a sequence of strings) containing
 * all elements of string table pointed by section header of given index.
 */
char *elf_parse_strtab_byindex(elf_half_t sh_idx, elf_word_t *n, elf_sym_t **syms)
{
    const elf_t *elfo = (const elf_t *)elf_get_my_elfo();
    ASSERT_ARG_RET_NULL(elf_validate_index(elfo, sh_idx));
    elf_shdr_t *shdr = elfo->shdrs[sh_idx];
    sbyte *strings = elf_parse_shstrtab(syms);
    if(strings != NULL && n != NULL) {
        *n = shdr->sh_size;
    }
    return strings;
}

/**
 * @brief Read .shstrtab section's string table.
 *
 * Contract:
 * @note This method depends on calling both _elf_parse_ehdr and
 * elf_parse_shdrs first.
 * @see elf_parse_ehdr
 * @see elf_parse_shdrs
 *
 * @return An array of bytes (hopefully, a sequence of strings) containing
 * all elements of .shstrtab string table.
 */
sbyte* elf_parse_shstrtab(elf_sym_t **syms)
{
    const elf_t *elfo = (const elf_t *)elf_get_my_elfo();
    ASSERT_CON_RET_NULL(elfo->ehdr && elfo->shdrs);
    if(elfo->ehdr->e_shstrndx == SHN_UNDEF) {
        log_debug("undefined string table");
        return NULL;
    }
    elf_shdr_t *shstrtab = elfo->shdrs[elfo->ehdr->e_shstrndx];
    return ((char*)elfo->mapaddr + shstrtab->sh_offset);
}

/**
 * @brief Read .dynstr section's string table.
 *
 * @return An array of bytes (hopefully, a sequence of strings) containing
 * all elements of .dynstr string table.
 */
unsigned char* elf_get_symname_offset(const char *name)
{
    const elf_t *elfo = (const elf_t *)elf_get_my_elfo();
    elf_shdr_t *sct = _elf_get_shdr_byname(elfo, name);
    if(sct == NULL) {
        log_debug("%s not found!", name);
        return NULL;
    }
    return ((unsigned char*)elfo->mapaddr + sct->sh_offset);
}

/** @} */

/* __EOF__ */
