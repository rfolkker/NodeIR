#include "InitPageGenerator.h"
InitPageGenerator::InitPageGenerator(){

pageFile = "<html>";
pageFile += "<head>";
pageFile += "    <title>Initialize WebIR</title>";
pageFile += "    <script>";
pageFile += "        function TriggerInit() {";
pageFile += "            $.ajax(\"http://192.168.4.1/irInit\", {";
pageFile += "                type: \"POST\",";
pageFile += "                dataType: 'json',";
pageFile += "                data: JSON.stringify('{\"irSSID\": "' + $(\"txtSSID\").val() + '", \"irPWD\": "' + $(\"txtPWD\").val() + '\"}'),";
pageFile += "                success: function() {";
pageFile += "                    alert(\"Connection triggered\");";
pageFile += "                }";
pageFile += "            })";
pageFile += "        }";
pageFile += "    </script>";
pageFile += "</head>";

pageFile += "<body>";
pageFile += "    <form>";
pageFile += "        <p>";
pageFile += "            <div><label>SSID:</label><input id=\"txtSSID\" type=\"text\" width=\"20\" /></div>";
pageFile += "        </p>";
pageFile += "        <p>";
pageFile += "            <div><label>Passphrase:</label><input id=\"txtPass\" type=\"text\" width=\"20\" /></div>";
pageFile += "        </p>";
pageFile += "        <div><input type=\"button\" value=\"Connect\" onclick=\"TriggerInit();\" />";
pageFile += "    </form>";
pageFile += "</body>";

pageFile += "</html>";
};
String InitPageGenerator::GetDefaultPageFile(){
    return pageFile;
}