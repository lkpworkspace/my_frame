#include "MySerialize.h"
#include <string.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// MyOutStream class

void MyOutStream::Write(const void* data, size_t bytes)
{
    uint32_t new_head = m_head + static_cast<uint32_t>(bytes);
    if(new_head > m_capacity)
    {
        ReallocBuf(std::max(m_capacity * 2,new_head));
    }
    memcpy(m_buf + m_head, data, bytes);
    m_head = new_head;
}

void MyOutStream::ReallocBuf(uint32_t new_len)
{
    m_buf = static_cast<char*>(std::realloc(m_buf,new_len));
    if(!m_buf)
    {
        printf("realloc failed\n");
    }
    m_capacity = new_len;
}
