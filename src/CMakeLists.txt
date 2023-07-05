cmake_minimum_required(VERSION 3.5)

PROJECT(TRANSFERTOOL)

ADD_EXECUTABLE(TransferServer serverMain.cpp messageTransferServer.cpp fileTransferServer.cpp tcpSocket.cpp udpSocket.cpp dataPackage.cpp dirReader.cpp cJSON.c)
ADD_EXECUTABLE(TransferClient clientMain.cpp messageTransferClient.cpp fileTransferClient.cpp commandAnaly.cpp udpSocket.cpp tcpSocket.cpp cJSON.c dataPackage.cpp)

target_link_libraries(TransferServer ws2_32)
target_link_libraries(TransferClient ws2_32)
