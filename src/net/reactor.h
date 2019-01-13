#ifndef BRKS_INTF_INTERFACE_H_
#define BRKS_INTF_INTERFACE_H_

// #include "protocol_head.h"
// #include "protocol_codec.h"
// #include "json_protocol_codec.h"

// #include "../common/event.h"
// #include "../common/protocol_codec.h"

#include "iomodel.h"

class CReactor : public CThread
{
public:
    CReactor();
    virtual ~CReactor();
	virtual void run();

public:
    bool fire();
    void misfire();

	bool add_socket(YI_SOCKET sockfd, int events, void* key);
	bool del_socket(YI_SOCKET sockfd);
	bool modify_socket(YI_SOCKET sockfd, int events, void* key);

    void push_event(IEvent* ev) { que_of_event_.push(ev); }
	void push_delayevent(IEvent* ev) { que_of_delay_event_.push(ev); }

protected:
    void do_event_queue();

private:
    CIOModel io_model_;
	CEventQueue<IEvent *> que_of_event_;
	CEventQueue<IEvent *> que_of_delay_event_;
};

#endif