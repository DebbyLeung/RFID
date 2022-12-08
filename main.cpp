#include "include/rfid.h"
#include "curl/curl.h"


const char* HOST = "172.18.0.103";
const char* PORT = "1234";

const uint16_t device_id = 255;

std::string get_url(Rfid &rfid) {
	//http
	std::string str2 = "cmd=";
	//get route
	str2.append(rfid.get_str('s'));
	
	//std::cout << link << str2 << std::endl;
	return str2;
}

std::vector<BYTE> split_int(uint16_t val) {
	BYTE high = (BYTE)((val & 0xFF00) >> 8);
	BYTE low = (BYTE)(val & 0x00FF);
	std::vector<BYTE> arr = { high , low };
	return  arr;
}

int main() {
	std::string link = HOST;
	link.append(":");
	link.append(PORT);
	link = "172.18.0.103:1234/on";
	Rfid rfid;

	bool post = false;
	CURL* curl;
	CURLcode res;

	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();
	if (curl) {
		/* First set the URL that is about to receive our POST. This URL can
		   just as well be an https:// URL if that is what should receive the
		   data. */
		curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
		if (post) {
			std::string cmd = get_url(rfid);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cmd.c_str());
		}
		std::string readBuffer;
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		}
		else {
			char* url;
			curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);
			if (url){
				printf("Redirect to: %s\n", url);
				printf("%s", readBuffer.c_str());
			}
			/* always cleanup */
			curl_easy_cleanup(curl);
		}
		curl_global_cleanup();
		return 0;

	}
}
