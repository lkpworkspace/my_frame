#ifndef MYSERIALIZE_H
#define MYSERIALIZE_H
#include "MyCommon.h"

#define offset_of(c, mv) ((size_t)&(static_cast<c*>(nullptr)->mv))

enum EBaseType
{
    EBT_CHAR,
    EBT_SHORT,
    EBT_INT,
    EBT_FLOAT,
    EBT_DOUBLE,
    EBT_STRING
};




class MyMemVar
{
public:
    MyMemVar(const char* name,
             EBaseType ebt, uint32_t offset):
        m_name(name),m_ebt(ebt),m_offset(offset){}

    EBaseType GetBaseType() const { return m_ebt; }
    uint32_t GetOffset() const { return m_offset; }
private:
    EBaseType m_ebt;
    uint32_t m_offset;
    std::string m_name;
};




class MyData
{
public:
    MyData(std::initializer_list<MyMemVar>& mmv):
        m_mmv(mmv){}

    const std::vector<MyMemVar>& GetMemVar() const { return m_mmv; }
private:
    std::vector<MyMemVar> m_mmv;
};



class MyIOStream
{
public:
    virtual void Serialize(void* data, uint32_t bytes) = 0;
    virtual bool IsInput() = 0;
};

class MyOutStream : public MyIOStream
{
public:
    MyOutStream():
        m_buf(nullptr),m_head(0),m_capacity(0)
    {ReallocBuf(32);}
    ~MyOutStream()
    {free(m_buf);}

    /// override
    virtual void Serialize(void* data, uint32_t bytes){Write(data,bytes);}
    virtual bool IsInput(){return false;}
    /// override end

    const char* GetPtr(){return m_buf;}
    uint32_t GetLen(){return m_head;}

    void Write(const void* data, size_t bytes);

private:
    void ReallocBuf(uint32_t new_len);

    char* m_buf;
    uint32_t m_head;
    uint32_t m_capacity;
};

void Serialize(MyIOStream* ios, const MyData* data, uint8_t* in_data)
{
    for(auto& mv: data->GetMemVar())
    {
        void* mvData = in_data + mv.GetOffset();
        switch(mv.GetBaseType())
        {
        case EBT_CHAR:
            ios->Serialize(mvData,sizeof(char));break;
        case EBT_SHORT:
            ios->Serialize(mvData,sizeof(short));break;
        case EBT_INT:
            ios->Serialize(mvData,sizeof(int));break;
        case EBT_FLOAT:
            ios->Serialize(mvData,sizeof(float));break;
        case EBT_DOUBLE:
            ios->Serialize(mvData,sizeof(double));break;
        case EBT_STRING:
            //ios->Serialize(mvData,((std::string*)mvData)->size() + 1);
            break;
        }
    }
}

#endif // MYSERIALIZE_H
