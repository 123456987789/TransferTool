#include "../header/messageTransferServer.hpp"
#include "../header/fileTransferServer.hpp"

#include <thread>

const char* ip = "127.0.0.1";
const uint16_t msg_port = 39000;
const uint16_t fil_port = 39001;

messageTransferServer mts;
fileTransferServer fts(ip, fil_port);

void thread_mts()
{
    mts.messageTransferServerStartUp(ip, msg_port);
}

void thread_fts()
{
    fts.fileTransferServerStartUp();
}

int main()
{
    std::thread th_mts(thread_mts);
    std::thread th_fts(thread_fts);
    th_fts.detach();
    th_mts.join();
    return 0;
}
