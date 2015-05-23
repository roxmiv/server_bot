#include "server_api.h"

int StrToInt(const std::string& str)
{
    std::stringstream ss(str);
    int ret;
    ss >> ret;
    return ret;

}

void ServerAPI::Connect()
{
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in dest;

    memset(&dest, 0, sizeof(dest));

    dest.sin_family = AF_INET;

    inet_pton(AF_INET, m_ip.c_str(), &dest.sin_addr);
    dest.sin_port = htons(m_port);

    connect(m_sockfd, (struct sockaddr*)&dest, sizeof(struct sockaddr));
    std::thread* m_watch = new std::thread(std::bind(&ServerAPI::Watcher, this));
}

void ServerAPI::Watcher()
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

void ServerAPI::ParseData( std::string data)
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
    if ((rpos = data.find("players are in business")) != std::string::npos)
    {
        lpos = data.rfind("\n", rpos);
        std::string activePlayers = data.substr(lpos, rpos - lpos);
        m_activePlayers = StrToInt(activePlayers);
    }

    if ((lpos = data.find("Your number is ")) != std::string::npos)
    {
        rpos = data.find(".");
        lpos = lpos + std::string("Your number is ").length();
        std::string id = data.substr(lpos,  rpos - lpos);
        m_myid = StrToInt(id);
    }

    std::regex market_regex("level = ([0-9]+): ([0-9]+) stuff for \\$([0-9]+).*; ([0-9]+).*\\$([0-9]*)");
    std::smatch market_match;

    if (std::regex_search(data, market_match, market_regex))
    {
        m_turn = StrToInt(market_match[1].str());
        m_supply = StrToInt(market_match[2].str());
        m_rawPrice = StrToInt(market_match[3].str());
        m_demand = StrToInt(market_match[4].str());
        m_productionPrice = StrToInt(market_match[5].str());


    }

    std::regex inf_regex("Number.*Balance\\n([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)");
    std::smatch inf_match;

    if (std::regex_search(data, inf_match, inf_regex))
    {
        PlayerData &data = m_players[StrToInt(inf_match[1].str())];

        data.raw = StrToInt(inf_match[2].str());
        data.production = StrToInt(inf_match[3].str());
        data.factories = StrToInt(inf_match[4].str());
        data.money = StrToInt(inf_match[5].str());


    }

    if (data.find("New month") != std::string::npos)
    {
        {
            std::unique_lock<std::mutex> lock(m_mtx);

            m_newTurnCv.notify_all();
            m_newTurn = true;
        }

        for (std::map<int, PlayerData>::iterator it = m_players.begin(); it != m_players.end(); ++it)
        {
            it->second.manufactured = 0;
            it->second.result_raw_sold = 0;
            it->second.result_raw_price = 0;
            it->second.result_prod_bought = 0;
            it->second.result_prod_price = 0;
       }
        
        std::stringstream ss(data);
        std::string tmp;
        std::regex buy_regex("bought\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)");
        std::regex sold_regex("sold\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)");

        while (getline(ss, tmp))
        {
            std::smatch buy_match;
            if (std::regex_search(tmp, buy_match, buy_regex))
            {
                PlayerData &data = m_players[StrToInt(buy_match[1].str())];
                data.result_raw_sold = StrToInt(buy_match[2].str());
                data.result_raw_price = StrToInt(buy_match[3].str());
            }
            std::smatch sold_match;
            if (std::regex_search(tmp, sold_match, sold_regex))
            {
                PlayerData &data = m_players[StrToInt(sold_match[1].str())];
                data.result_prod_bought = StrToInt(sold_match[2].str());
                data.result_prod_price = StrToInt(sold_match[3].str());
            }
        }
    
    }



    

}

//int main()
//{
    //ServerAPI server(std::string("127.0.0.1"), 14880);

    //server.Connect();

    //server.End();
    ////std::string ololo;
    ////while (1)
    ////{
        ////std::cin >> ololo;
        ////std::cout << "COMMAND = " << ololo << std::endl;
        ////ololo += '\n';
        ////send(sock, ololo.c_str(), ololo.size(), 0);
    ////}

//}
