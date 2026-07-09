/*
 * Kenux Kernel Open v26.7.9O — ldso.h
 * Copyright (c) 2026 HGSpace Studio. MIT License.
 * Developer: happygray110
 *
 * [HG-LDSO] 动态链接器头文件。
 *            定义 ELF 动态链接所需的所有数据结构和常量。
 *            用户态 ELF 加载器调用 ldso 加载 .so 共享库，
 *            解析符号、执行重定位、调用 .init/.fini。
 *
 *            重定位类型 R_KENUX_* 是自定义的，对标 x86_64 ELF 的 R_X86_64_*，
 *            但 Kenux 用自己的命名空间避免与 glibc/编译器内建宏冲突。
 */

#ifndef ARCH_LDSO_H  /* [HG-GUARD] 防重复包含 */
#define ARCH_LDSO_H

#include <arch/types.h>     /* [HG-TYPES] uint64_t/int64_t 等 */
#include <arch/spinlock.h>  /* [HG-LOCK]  spinlock_t，ldso 全局状态锁 */

/* [HG-LDSO-PATH] 动态链接器自身路径，ELF 可执行文件的 PT_INTERP 指向此路径 */
#define LDSO_PATH              "/lib/ld-kenux.so"
/* [HG-LDSO-MAXLIBS] 最多同时加载的共享库数量，超过则返回错误 */
#define LDSO_MAX_LIBS          64
/* [HG-LDSO-MAXSYMS] 全局符号表最大条目数（所有库的 symtab 合计） */
#define LDSO_MAX_SYMS          4096
/* [HG-LDSO-MAXRELS] 全局重定位表最大条目数 */
#define LDSO_MAX_RELS          8192
/* [HG-LDSO-NAMELEN] 库文件名最大长度 */
#define LDSO_LIB_NAME_MAX      256

/* === [HG-LDSO-RELOC] Kenux 自定义重定位类型 === */
#define R_KENUX_64             1    /* [HG] 绝对 64 位重定位，S + A */
#define R_KENUX_PC32           2    /* [HG] 32 位 PC 相对重定位，S + A - P */
#define R_KENUX_GLOB_DAT       3    /* [HG] GOT 全局条目，解析符号地址填入 */
#define R_KENUX_JUMP_SLOT      4    /* [HG] PLT 跳转槽，延迟绑定目标 */
#define R_KENUX_RELATIVE       5    /* [HG] 基址相对重定位，B + A（不需要符号解析） */

/* === [HG-LDSO-DTAG] ELF dynamic section tag 常量 === */
#define DT_NULL                0    /* [HG] 数组结束标记 */
#define DT_NEEDED              1    /* [HG] d_val = 依赖库名字符串偏移 */
#define DT_PLTRELSZ            2    /* [HG] PLT 重定位表总字节数 */
#define DT_PLTGOT              3    /* [HG] GOT 表地址（.got.plt 段） */
#define DT_HASH                4    /* [HG] 符号哈希表地址 */
#define DT_STRTAB              5    /* [HG] 动态字符串表地址 */
#define DT_SYMTAB              6    /* [HG] 动态符号表地址 */
#define DT_STRSZ               10   /* [HG] 动态字符串表大小 */
#define DT_SYMENT              11   /* [HG] 单个符号表项大小（24 字节） */
#define DT_INIT                12   /* [HG] .init 函数地址 */
#define DT_FINI                13   /* [HG] .fini 函数地址 */
#define DT_REL                 17   /* [HG] REL 重定位表地址 */
#define DT_RELSZ               18   /* [HG] REL 重定位表大小 */
#define DT_RELENT              19   /* [HG] 单个 REL 条目大小 */
#define DT_PLTREL              20   /* [HG] PLT 重定位类型（RELA=7） */
#define DT_JMPREL              23   /* [HG] PLT RELA 重定位表地址 */
#define DT_BIND_NOW            24   /* [HG] 立即绑定标志 */
#define DT_INIT_ARRAY          25   /* [HG] .init_array 数组地址 */
#define DT_FINI_ARRAY          26   /* [HG] .fini_array 数组地址 */
#define DT_INIT_ARRAYSZ        27   /* [HG] .init_array 条目数 */
#define DT_FINI_ARRAYSZ        28   /* [HG] .fini_array 条目数 */
#define DT_FLAGS               30   /* [HG] 标志位 */
#define DT_FLAGS_1             0x6ffffffb  /* [HG] 扩展标志位 */

/* [HG] DF_BIND_NOW: 所有重定位在加载时完成，禁用延迟绑定 */
#define DF_BIND_NOW            0x8

/*
 * [HG-LDSO-SYM] ELF 符号表项结构（24 字节，与 ELF64_Sym 布局一致）。
 */
typedef struct {
    uint32_t st_name;    /* [HG] 符号名在 strtab 中的偏移 */
    uint8_t  st_info;    /* [HG] 绑定+类型：(bind<<4)|type */
    uint8_t  st_other;   /* [HG] 符号可见性（STV_DEFAULT/STV_HIDDEN） */
    uint16_t st_shndx;   /* [HG] 所在段索引，SHN_UNDEF=未定义（需重定位） */
    uint64_t st_value;   /* [HG] 符号值/地址 */
    uint64_t st_size;    /* [HG] 符号大小（字节） */
} kelf_sym_t;

/* [HG-LDSO-SYM-BIND] 符号绑定类型，取自 st_info 高 4 位 */
#define KELF_STB_GLOBAL        1    /* [HG] 全局符号，跨库可见 */
#define KELF_STB_WEAK          2    /* [HG] 弱符号，可被强符号覆盖 */

/* [HG-LDSO-SYM-TYPE] 符号类型，取自 st_info 低 4 位 */
#define KELF_STT_NOTYPE        0    /* [HG] 未指定类型 */
#define KELF_STT_OBJECT        1    /* [HG] 数据对象（变量） */
#define KELF_STT_FUNC          2    /* [HG] 函数 */

/* [HG-LDSO-SYM-MACRO] 从 st_info 提取绑定和类型的位操作 */
#define KELF_ST_BIND(info)     ((info) >> 4)       /* [HG] 高 4 位 = 绑定 */
#define KELF_ST_TYPE(info)     ((info) & 0xf)      /* [HG] 低 4 位 = 类型 */

/*
 * [HG-LDSO-RELA] ELF RELA 重定位条目（带 addend，24 字节）。
 *                  x86_64 只用 RELA，不用 REL（无隐式 addend）。
 */
typedef struct {
    uint64_t r_offset;  /* [HG] 需要重定位的虚拟地址（GOT/数据段） */
    uint64_t r_info;    /* [HG] 高 32 位=符号表索引，低 32 位=重定位类型 */
    int64_t  r_addend;  /* [HG] 显式加数 A */
} kelf_rela_t;

/* [HG-LDSO-RELA-MACRO] 从 r_info 提取符号索引和重定位类型 */
#define KELF_R_SYM(info)       ((uint32_t)((info) >> 32))       /* [HG] 高 32 位 = sym index */
#define KELF_R_TYPE(info)      ((uint32_t)((info) & 0xffffffffUL))  /* [HG] 低 32 位 = type */
#define KELF_R_INFO(sym, type) (((uint64_t)(sym) << 32) | ((uint64_t)(type)))  /* [HG] 组合 */

/*
 * [HG-LDSO-HASH] ELF SYSV 哈希表头。
 *                  nbucket 个桶 + nchain 个链，用于快速符号查找。
 */
typedef struct {
    uint32_t nbucket;  /* [HG] 哈希桶数量 */
    uint32_t nchain;   /* [HG] 哈希链长度（= 符号表条目数） */
} kelf_hash_t;

/*
 * [HG-LDSO-DYN] ELF dynamic 段条目。d_tag 决定 d_val/d_ptr 的含义。
 */
typedef struct {
    int64_t  d_tag;    /* [HG] 标签：DT_NEEDED/DT_STRTAB/... */
    union {
        uint64_t d_val;  /* [HG] 整数值 */
        uint64_t d_ptr;  /* [HG] 指针值 */
    } d_un;
} kelf_dyn_t;

/*
 * [HG-LDSO-KLIB] 已加载共享库的运行时描述。
 *                  每个加载的 .so 占一个 klib_t。
 */
typedef struct klib {
    uint64_t     base_addr;        /* [HG] 库在虚拟地址空间的加载基址 */
    uint64_t     load_size;        /* [HG] 库占用的虚拟内存大小 */
    char         name[LDSO_LIB_NAME_MAX];  /* [HG] 库文件名 */
    int          fd;               /* [HG] 打开的文件描述符 */

    kelf_sym_t*  symtab;           /* [HG] 符号表指针 */
    uint32_t     sym_count;        /* [HG] 符号表条目数 */
    const char*  strtab;           /* [HG] 字符串表指针 */
    uint64_t     strtab_size;      /* [HG] 字符串表大小 */
    kelf_hash_t* hash;             /* [HG] SYSV 哈希表 */
    uint32_t     hash_nbucket;     /* [HG] 哈希桶数 */
    uint32_t     hash_nchain;      /* [HG] 哈希链长度 */

    kelf_rela_t* plt_rels;         /* [HG] PLT 重定位表（延迟绑定） */
    uint64_t     plt_relsz;        /* [HG] PLT 重定位表总字节数 */
    kelf_rela_t* rels;             /* [HG] 非 PLT 重定位表（立即绑定） */
    uint64_t     relsz;            /* [HG] 非 PLT 重定位表总字节数 */

    void        (*init_func)(void);  /* [HG] .init 函数指针 */
    void        (*fini_func)(void);  /* [HG] .fini 函数指针 */
    void**       init_array;        /* [HG] .init_array 函数指针数组 */
    uint32_t     init_array_sz;    /* [HG] .init_array 条目数 */
    void**       fini_array;        /* [HG] .fini_array 函数指针数组 */
    uint32_t     fini_array_sz;    /* [HG] .fini_array 条目数 */

    int          ref_count;        /* [HG] 引用计数，0 时可卸载 */
    int          resolved;         /* [HG] 符号解析完成标志 */
    int          initialized;      /* [HG] .init 已调用标志 */

    uint64_t     map_start;        /* [HG] mmap 区域起始地址 */
    uint64_t     map_end;          /* [HG] mmap 区域结束地址 */
} klib_t;

/*
 * [HG-LDSO-SYMCACHE] 符号查找缓存（256 条，LRU）。
 */
typedef struct {
    const char*  name;    /* [HG] 符号名 */
    uint64_t     addr;    /* [HG] 解析后的符号地址 */
    klib_t*      owner;   /* [HG] 该符号属于哪个库 */
    uint32_t     hits;    /* [HG] 缓存命中次数（LRU 依据） */
} sym_cache_entry_t;

/* [HG] 缓存大小 256 条 */
#define LDSO_SYM_CACHE_SIZE      256

/*
 * [HG-LDSO-STATE] ldso 全局状态单例。
 */
typedef struct {
    klib_t              libs[LDSO_MAX_LIBS];             /* [HG] 已加载库数组 */
    sym_cache_entry_t   sym_cache[LDSO_SYM_CACHE_SIZE];  /* [HG] 符号查找缓存 */
    spinlock_t          lock;          /* [HG] 全局自旋锁，保护所有字段 */
    uint32_t            lib_count;     /* [HG] 已加载库数量 */
    uint64_t            total_relocs;  /* [HG] 累计重定位次数 */
    uint64_t            total_lookups; /* [HG] 累计符号查找次数 */
    uint64_t            cache_hits;    /* [HG] 缓存命中次数 */
} ldso_state_t;

/* [HG-LDSO-INIT] 初始化 ldso_state 结构 */
void ldso_init_state(ldso_state_t* state);

/* [HG-LDSO-GETSTATE] 获取全局 ldso_state 单例指针 */
ldso_state_t* ldso_get_state(void);

/* [HG-LDSO-LOAD] 加载一个共享库到内存 */
klib_t* ldso_load_library(const char* path, ldso_state_t* state);

/* [HG-LDSO-LOOKUP] 在所有已加载库中查找符号，返回地址（0=未找到） */
uint64_t ldso_lookup_symbol(const char* name, ldso_state_t* state);

/* [HG-LDSO-RELOC] 对单个库执行重定位 */
int ldso_relocate(klib_t* lib, ldso_state_t* state);

/* [HG-LDSO-INITCALL] 遍历所有库，调用 .init 和 .init_array */
void ldso_call_init(ldso_state_t* state);

/* [HG-LDSO-FINICALL] 遍历所有库，调用 .fini 和 .fini_array */
void ldso_call_fini(ldso_state_t* state);

/* [HG-LDSO-UNLOAD] 卸载共享库 */
int ldso_unload_library(klib_t* lib, ldso_state_t* state);

#endif /* ARCH_LDSO_H */