// Fixed width header object for simple use
#ifndef HEADERS_H
#define HEADERS_H

#include <Arduino.h>
class Headers{
	private:
		String _Keys[8]= {
			"Host: ",
			"User-Agent: ",
			"Accept: ",
			"Accept-Language: ",
			"Accept-Encoding: ",
			"Content-Type: ",
			"Content-Length: ", 
			"Connection: "};
    int _KeyCount=8;
		String _Host;
		String _UserAgent;
		String _Accept;
		String _AcceptLanguage;
		String _AcceptEncoding;
    String _ContentType;
		int _ContentLength;
		String _ContentLengthText;
		String _Connection;
	public:
		Headers();
		bool Add(String newData);
		String GetHost(){return _Host;};
		String GetUserAgent(){return _UserAgent;};
		String GetAccept(){return _Accept;};
		String GetAcceptLanguage(){return _AcceptLanguage;};
		String GetAcceptEncoding(){return _AcceptEncoding;};
    String GetContentType(){return _ContentType;};
		int GetContentLength(){return _ContentLength;};
    String GetContentLengthText(){return _ContentLengthText;};
		String GetConnection(){return _Connection;};
};

#endif
