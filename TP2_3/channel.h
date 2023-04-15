#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <systemc>

struct Channel
{
	Channel()
	{
		reset();
	}


	//Prepare the channel (when first task arrives)
	void set(char* buffer, int buffer_size)
	{
		//The channel should be empty when set is called (or content will be overwriten)
		assert(m_empty);
		this->m_buffer=buffer;
		this->m_buffer_size=buffer_size;
		this->m_empty=false;
	}

	//Is the channel prepared (i.e. the first task already came)
	bool isSet()
	{		
		return !m_empty;
	}


	//Clear the channel (after buffer copy)
	void reset()
	{
		m_empty=true;
		m_buffer=NULL;
	}



	char* getBuffer()
	{
		return m_buffer;
	}

	
	int getBufferSize()
	{
		return m_buffer_size;
	}


	sc_event m_event; //The event triggered when the second process reaches the rendezvous
		
private:

	bool m_empty; //True before the first process arrives, false afterwards
	char* m_buffer; //Pointer to the data to copy
	int m_buffer_size; //Size of the buffer data to transfer

};


#endif
