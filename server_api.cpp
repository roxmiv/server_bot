#include <iostream>

#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sstream>

#include <vector>
#include <errno.h>

#include <condition_variable>
#include <thread>
#include <mutex>
#include <string>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
class ServerAPI
{
public:


    ServerAPI(const std::string& ip, unsigned int port)
    {
       m_ip = ip;
       m_port = port;

    }

    void Connect()
    {
        m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    }


    void request()
    {
        
    }


private:
    int m_sockfd;

    std::string   m_ip;
    unsigned int  m_port;


};


struct PlayerData
{
    PlayerData()
        : money(0)
        , raw(0)
        , production(0)
        , factories(0)
        , manufactured(0)
    {

    }
    unsigned int money;
    unsigned int raw;
    unsigned int production;
    unsigned int factories;
    unsigned int manufactured;
};


struct LastTurn
{
    LastTurn()
        : result_raw_sold(0)
        , result_raw_price(0)
        , result_prod_bought(0)
        , result_prod_price(0)
    {

    }
    unsigned int result_raw_sold;
    unsigned int result_raw_price;
    unsigned int result_prod_bought;
    unsigned int result_prod_price;
};

class GameState
{
public:
    GameState(int sockfd)
        :m_sockfd(sockfd)
    {
    }

    void Watcher()
    {
        char buf[1024];
        fd_set readSet;
        int result;
        int iof = -1;
        while (1)
        {
            do {
                FD_ZERO(&readSet);
                FD_SET(m_sockfd, &readSet);
                result = select(m_sockfd + 1, &readSet, NULL, NULL, NULL);
            } while (result == -1 && errno == EINTR);
            char* pbuf = buf;
            if (result > 0)
            {
                if (FD_ISSET(m_sockfd, &readSet))
                {
                    while (1)
                    {
                        if ((iof = fcntl(m_sockfd, F_GETFL, 0)) != -1)
                            fcntl(m_sockfd, F_SETFL, iof | O_NONBLOCK);

                        result = recv(m_sockfd, pbuf, 1024, 0);
                        if (result <= 0)
                            break;
                        pbuf = pbuf + result;
                            
                   }
                   std::string res(buf, pbuf - buf);
                   ParseData(res);

                }

            }
            pbuf = buf;
        }
        
    }

    void ParseData( std::string data)
    {
        for (int i = 0; i < data.length(); ++i)
        {
            if (data[i] == '\0')
                data = data.erase(i, 1);
        }

        if (data.find("You don't have enough resources") != std::string::npos)
        {
        }
        size_t rpos = 0;
        size_t lpos = 0;
        if ((rpos = data.find("players are in business") > 0))
        {
            lpos = data.rfind("\n");
            std::string activePlayer = data.substr(lpos, rpos - lpos);
        }

    }
    //void WaitNewTurn()
    //{
        //std::unique_lock<std::mutex> lock(m_mtx);

        //while (!m_newTurn)
        //{
            //m_newTurnCv.wait(lock);
        //}
        

    //}

private:
    int m_sockfd;

    unsigned int m_myid;
    unsigned int m_turn;
    unsigned int m_activePlayers;
    unsigned int m_supply;
    unsigned int m_rawPrice;
    unsigned int m_demand;
    unsigned int m_productionPrice;

    unsigned int* m_moneyArr;
    unsigned int* m_rawArr;
    unsigned int* m_production;

    std::vector<PlayerData> m_players;

//    LastTurn lastTurn;
    
//std::condition_variable m_newTurnCv;
//    bool m_newTurn;
//    std::mutex m_mtx;

    

};

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in dest;

    memset(&dest, 0, sizeof(dest));

    dest.sin_family = AF_INET;

    dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    dest.sin_port = htons(14880);

    connect(sock, (struct sockaddr*)&dest, sizeof(struct sockaddr));

    GameState game(sock);

    std::thread thr(std::bind(&GameState::Watcher, &game));
    std::string ololo;
    while (1)
    {
        std::cin >> ololo;
        std::cout << "COMMAND = " << ololo << std::endl;
        ololo += '\n';
        send(sock, ololo.c_str(), ololo.size(), 0);
    }
    thr.join();

}
