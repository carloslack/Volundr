#include "destroy.h"

//bool destroy_meta(elf_t* elf) 
//{
//    bool ret = false;
//    if(elf) {
//        elf_t **e = &elf;
//        sfree((void**)&(*e)->meta);
//        if(&(*e)->meta)
//            ret = true;
//    }
//    return ret;
//}

//bool elf_destroy_header(elf_t* elf) 
//{
//    bool ret = false;
//    if(elf) {
//        elf_t **e = &elf;
//        sfree((void**)&(*e)->ehdr);
//        if(&(*e)->ehdr)
//            ret = true;
//    }
//    return ret;
//}

bool elf_destroy_program(elf_t* elf) 
{
    bool ret = false;
    if(elf) {
        elf_t **e = &elf;
        sfree((void**)&(*e)->phdrs);
        if(&(*e)->phdrs)
            ret = true;
    }
    return ret;
}

bool elf_destroy_section(elf_t* elf) 
{
    bool ret = false;
    if(elf) {
        elf_t **e = &elf;
        sfree((void**)&(*e)->shdrs);
        if(&(*e)->shdrs)
            ret = true;
    }
    return ret;
}

//bool destroy_file(elf_t* elf) 
//{
//    bool ret = false;
//    if(elf) {
//        elf_t **e = &elf;
//        sfree((void**)&(*e)->fp);
//        if(&(*e)->fp)
//            ret = true;
//    }
//    return ret;
//}
