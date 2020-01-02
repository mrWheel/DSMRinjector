

bool _dThis = false;

#define Debug(...)      ({ if (_dThis == true) { \
                                                            TelnetStream.printf("%s(%d): ",__FUNCTION__, __LINE__); \
                                                            _dThis = false;  \
                                                } \
                           TelnetStream.print(__VA_ARGS__); \
                           TelnetStream.flush(); \
                        })
#define Debugln(...)    ({ if (_dThis == true) { \
                                                            TelnetStream.printf("%s(%d): ",__FUNCTION__, __LINE__); \
                                                            _dThis = false; \
                                                } \
                           TelnetStream.println(__VA_ARGS__); \
                           TelnetStream.flush(); \
                        })
#define Debugf(...)     ({ if (_dThis == true) { \
                                                            TelnetStream.printf("%s(%d): ",__FUNCTION__, __LINE__); \
                                                            _dThis = false; \
                                                } \
                           TelnetStream.printf(__VA_ARGS__); \
                           TelnetStream.flush(); \
                        })
#define DebugFlush()    ({ TelnetStream.flush(); })
