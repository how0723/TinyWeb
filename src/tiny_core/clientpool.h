/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---Client class---
 *This class have same function like Server
 ****************************************
 *
 */

#ifndef CLIENT_POOL_T
#define CLIENT_POOL_T

#include <tiny_core/callback.h>
#include <tiny_core/netaddress.h>
#include <tiny_core/protocol.h>

#include <deque>
#include <map>
#include <set>

class EventLoop;
class Connector;
class Connection;
class Factory;

class ClientPool {
private:
    EventLoop* m_pEventLoop;

    bool m_nStarted;
    unsigned long long m_nConNum;
    NetAddress m_nHostAddress;
    unsigned int m_nCapacity;
    unsigned int m_nUsing;

    typedef std::pair<NetAddress, NetAddress> AddressCouple;
    typedef std::pair<Connector*, Connection*> ConnectorCouple;
    typedef std::deque<ConnectorCouple> ConnectorCouples;
    typedef std::pair<AddressCouple, Protocol*> MultiProtocol;

    std::map<AddressCouple, ConnectorCouples> m_nConnections;
    std::map<Connection*, Protocol*> m_nProtocols;
    std::deque<MultiProtocol> m_nWaitList;

    void m_nConnectCallback(Connection* con);
    void m_nMessageCallback(Connection* con,
        Buffer* input, Time time);
    void m_nWriteCompleteCallback(Connection* con);
    void m_nCloseCallback(Connection* con);

    void m_fNewConnectionCallback(int, const NetAddress&,
        const NetAddress&);
    void m_fHandleClose(Connection*);

public:
    ClientPool(EventLoop*, const NetAddress&);

    virtual void buildProtocol(Protocol* prot) {}
    void closeProtocol(Protocol*);                   // used by protocol
    void closeProtocolAfter(Protocol*, int seconds); // used by protocol

    void addClient(const NetAddress& hostAddress,
        const NetAddress& peerAddress,
        bool retry,
        bool keepConnect,
        int num);

    void doTask(const NetAddress& hostAddress,
        const NetAddress& peerAddress,
        Protocol* protocol);

    void start();
    void connect(const NetAddress& hostAddress,
        const NetAddress& peeraddress,
        bool retry,
        bool keepconnect);
    void disconnect(const NetAddress& hostaddress,
        const NetAddress& peeraddress);
    void disconnectAll();
    void stop();
    ~ClientPool();
};

#endif // !CLIENT_POOL_T
