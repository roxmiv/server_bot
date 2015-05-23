

#include <sstream>


int StrToInt(const std::string& str)
{
    std::stringstream ss(str);
    int ret;
    ss >> ret;
    return ret;
    
}

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

    unsigned int result_raw_sold;
    unsigned int result_raw_price;
    unsigned int result_prod_bought;
    unsigned int result_prod_price;
};

class ServerAPI 
{
public:
    ServerAPI(std::string ip, int port)
        : m_ip(ip)
        , m_port(port)
    {
    }

    void Connect();

    void Watcher();

    void ParseData( std::string data);
    
        

    void WaitNewTurn()
    {
        std::unique_lock<std::mutex> lock(m_mtx);

        while (!m_newTurn)
        {
            m_newTurnCv.wait(lock);
        }

        m_newTurn = false;
    }    

    void Buy(int count, int cost)
    {
        std::stringstream ss;
        ss << "buy " << count << " " << cost << '\n';
        std::string str = ss.str();
        send(m_sockfd, &str[0], str.size(), 0 );
    }

    void Sell(int count, int cost)
    {
        std::stringstream ss;
        ss << "sell " << count << " " << cost << '\n';
        std::string str = ss.str();
        send(m_sockfd, &str[0], str.size(), 0 );
    }

    void Prod(int count)
    {
        std::stringstream ss;
        ss << "produce " << count << '\n';
        std::string str = ss.str();
        send(m_sockfd, &str[0], str.size(), 0 );
    }

    void NewPlant()
    {
        std::stringstream ss;
        ss << "newplant\n";
        std::string str = ss.str();
        send(m_sockfd, &str[0], str.size(), 0 );
    }

    void End()
    {
        std::stringstream ss;
        ss << "end\n";
        std::string str = ss.str();
        send(m_sockfd, &str[0], str.size(), 0 );
        WaitNewTurn();
    }

    void Market()
    {
        std::stringstream ss;
        ss << "market\n";
        std::string str = ss.str();
        send(m_sockfd, &str[0], str.size(), 0 );
    }

    unsigned int MyId()
    {
        return m_myid;
    }

    unsigned int Turn()
    {
        Market();
        sleep(1);
        return m_turn;
    }

    unsigned int ActivePlayers()
    {
        Market();
        sleep(1);
        return m_activePlayers;
    }

    unsigned int Supply()
    {
        Market();
        sleep(1);
        return m_supply;
    }

    unsigned int RawPrice()
    {
        Market();
        sleep(1);
        return m_rawPrice;
    }

    unsigned int Demand()
    {
        Market();
        sleep(1);
        return m_demand;
    }

    unsigned int ProductionPrice()
    {
        Market();
        sleep(1);
        return m_productionPrice;
    }


    unsigned int Money(int player)
    {   
        return m_players[player].money;

    }

    unsigned int Raw(int player)
    {
        return m_players[player].raw;
    }

    unsigned int Production(int player)
    {
        return m_players[player].production;
    }

    unsigned int Factories(int player)
    {
        return m_players[player].factories;
    }
    unsigned int Manufactured(int player)
    {
        return m_players[player].manufactured;
    }

    unsigned int ResultRawSold(int player)
    {
        return m_players[player].result_raw_sold;
    }

    unsigned int ResultRawPrice(int player)
    {
        return m_players[player].result_raw_price;
    }

    unsigned int ResultProdBought(int player)
    {
        return m_players[player].result_prod_bought;
    }

    unsigned int ResultProdPrice(int player)
    {
        return m_players[player].result_prod_price;
    }



private:
    int m_sockfd;
    std::string m_ip;
    int m_port;

    unsigned int m_myid;
    unsigned int m_turn;
    unsigned int m_activePlayers;
    unsigned int m_supply;
    unsigned int m_rawPrice;
    unsigned int m_demand;
    unsigned int m_productionPrice;


    std::map<int, PlayerData> m_players;

    std::condition_variable m_newTurnCv;
    bool m_newTurn;
    std::mutex m_mtx;

    std::thread* m_watch;
    

};
