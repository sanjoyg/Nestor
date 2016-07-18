#ifndef HTML_UTIL_H
#define HTML_UTIL_H

class HTMLUtil {
	public:
		static const char * getHomePreHTML() { return _homePreHTML; }
		static const char * getHomeConnectHTML() { return _homeConnectHTML; }
		static const char * getHomeMidHTML() { return _homeMidHTML; }
		static const char * getHomeSensorHTML() { return _homeSensorHTML; }
		static const char * getHomePostHTML() { return _homePostHTML; }
		
		static const char * getLogPreHTML() { return _logPreHTML; }
		static const char * getLogPostHTML() { return _logPostHTML; }
		
		static const char * getConfUploadHTML() { return _confUploadHTML; }
	
	private:
		HTMLUtil() {;}
		
		static const char *_homePreHTML;
		static const char *_homeConnectHTML;
		static const char *_homeMidHTML;
		static const char *_homeSensorHTML;
		static const char *_homePostHTML;

		static const char *_logPreHTML;
		static const char *_logPostHTML;
		
		static const char *_confUploadHTML;
};	

#endif