#define aslr(x) (x - 0x400000 + (DWORD)GetModuleHandle(0))
#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)

struct RBXTValue
{
    Value obj;
    std::uintptr_t pad;
    int type;
};

#define ls_top 16

#define R_LUA_TFUNCTION  9

#define lua_newthread_addr 0x01649310
#define lua_newthread_CCV __cdecl

#define lua_settop_addr 0x0164A7C0
#define lua_settop_CCV __cdecl

#define luau_deserialize_addr 0x01655490
#define luau_deserialize_CCV __cdecl

#define lua_pcall_addr 0x01649580
#define lua_pcall_CCV __cdecl

#define lua_topointer_addr 0x0164AB70
#define lua_topointer_CCV __cdecl

#define lua_pushlstring_addr 0x01649890
#define lua_pushlstring_CCV __cdecl

#define getdatamodel_addr 0x01077B30
#define getdatamodel_CCV __thiscall

#define getdatamodel2_addr 0x010779E0
#define getdatamodel2_CCV __cdecl

#define lua_setfield_addr 0x0164A4A0
#define lua_setfield_CCV __cdecl

typedef void(getdatamodel_CCV* DEF_getdatamodel1)(std::uintptr_t gdm2, std::uintptr_t pad);
DEF_getdatamodel1 getdatamodel1 = reinterpret_cast<DEF_getdatamodel1>(unprotect(aslr(getdatamodel_addr)));

typedef std::uintptr_t(getdatamodel2_CCV* DEF_getdatamodel2)();
DEF_getdatamodel2 getdatamodel2 = reinterpret_cast<DEF_getdatamodel2>(unprotect(aslr(getdatamodel2_addr)));

typedef std::uintptr_t(lua_newthread_CCV* DEF_lua_newthread)(std::uintptr_t state);
DEF_lua_newthread r_lua_newthread = reinterpret_cast<DEF_lua_newthread>(unprotect(aslr(lua_newthread_addr)));

typedef void(lua_settop_CCV* DEF_lua_settop)(std::uintptr_t state, int idx);
DEF_lua_settop r_lua_settop = reinterpret_cast<DEF_lua_settop>(unprotect(aslr(lua_settop_addr)));

typedef int(luau_deserialize_CCV* DEF_deserialize)(std::uintptr_t state, const char* chunkname, const char* buffer, size_t bufferlen, bool usestackenv);
DEF_deserialize deserialize = reinterpret_cast<DEF_deserialize>(unprotect(aslr(luau_deserialize_addr)));

typedef int(lua_pcall_CCV* DEF_lua_pcall)(std::uintptr_t state, int args, int resultidx, int errcallbackidx);
DEF_lua_pcall r_lua_pcall = reinterpret_cast<DEF_lua_pcall>(unprotect(aslr(lua_pcall_addr)));

typedef std::uintptr_t(lua_topointer_CCV* DEF_lua_topointer)(std::uintptr_t state, int idx);
DEF_lua_topointer r_lua_topointer = reinterpret_cast<DEF_lua_topointer>(unprotect(aslr(lua_topointer_addr)));

typedef void(lua_pushlstring_CCV* DEF_lua_pushlstring)(std::uintptr_t state, const char* buffer, size_t len);
DEF_lua_pushlstring r_lua_pushlstring = reinterpret_cast<DEF_lua_pushlstring>(unprotect(aslr(lua_pushlstring_addr)));

typedef void(lua_setfield_CCV* DEF_lua_setfield)(std::uintptr_t state, int idx, const char* name);
DEF_lua_setfield r_lua_setfield = reinterpret_cast<DEF_lua_setfield>(unprotect(aslr(lua_setfield_addr)));

namespace GDM {

    std::uintptr_t GetDataModel()
    {
        std::uintptr_t DMPad[8];
        getdatamodel1(getdatamodel2(), reinterpret_cast<std::uintptr_t>(DMPad));
        return DMPad[0] + 0xC;
    }

    int FindFirstClass(int Instance, const char* Name)
    {
        uintptr_t StartOfChildren = *reinterpret_cast<uintptr_t*>(Instance + 44);
        uintptr_t EndOfChildren = *reinterpret_cast<uintptr_t*>(StartOfChildren + 4);
        for (int i = *reinterpret_cast<int*>(StartOfChildren); i != EndOfChildren; i += 8)
        {
            if (!memcmp(reinterpret_cast<const char*>((*(int(**)(void))(*reinterpret_cast<int*>(*reinterpret_cast<int*>(i)) + 16))()), Name, strlen(Name)))
            {
                return *reinterpret_cast<int*>(i);
            }
        }
    }
}

void r_lua_pushrawobject(std::uintptr_t state, std::uintptr_t obj, int type)
{
    RBXTValue*& rawobj = *reinterpret_cast<RBXTValue**>(state + ls_top);
    rawobj->type = type;
    rawobj->obj.p = reinterpret_cast<void*>(obj);
    (rawobj)++;
}

DWORD RBX_LuaState(DWORD ScriptContext) { return  *(DWORD*)(ScriptContext + 276) + (ScriptContext + 276); }
DWORD RBX_SetIdentity(DWORD LS, int identity) { *(DWORD*)(*(DWORD*)(LS + 108) + 24) = identity; return 0; }