#include "Arduino.h"
#include "ESP8266WebServer.h"
#include "ESP8266mDNS.h"
#include "ArduinoJson.h"


#ifndef LISTEN_PORT
#define LISTEN_PORT 80
#endif

namespace wordclock
{
    class Webserver
    {
    public:
        Webserver(int listen_port = LISTEN_PORT);

        bool init();
        void loop();

        void apiSendJSON(int status);
        void apiSendOK();
        void apiSendError(String message);

        void apiGetStatus();
        void apiGetDateTime();
        void apiSetDateTime();
        void apiSetBrightness();
        void apiSetHostname();
        void apiResetConfig();

        using jsonDoc_t = StaticJsonDocument<10000> ;

    private:
        ESP8266WebServer            m_server;
        jsonDoc_t                   m_jsonDoc;
    };

    extern Webserver webserver;

}