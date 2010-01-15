#ifndef YYERRCODE
#define YYERRCODE 256
#endif

#define T_INTERFACE 257
#define T_PREFIX 258
#define T_ROUTE 259
#define T_RDNSS 260
#define T_NPMAP 261
#define STRING 262
#define NUMBER 263
#define SIGNEDNUMBER 264
#define DECIMAL 265
#define SWITCH 266
#define IPV6ADDR 267
#define INFINITY 268
#define T_IgnoreIfMissing 269
#define T_AdvSendAdvert 270
#define T_MaxRtrAdvInterval 271
#define T_MinRtrAdvInterval 272
#define T_MinDelayBetweenRAs 273
#define T_AdvManagedFlag 274
#define T_AdvOtherConfigFlag 275
#define T_AdvLinkMTU 276
#define T_AdvReachableTime 277
#define T_AdvRetransTimer 278
#define T_AdvCurHopLimit 279
#define T_AdvDefaultLifetime 280
#define T_AdvDefaultPreference 281
#define T_AdvSourceLLAddress 282
#define T_AdvOnLink 283
#define T_AdvAutonomous 284
#define T_AdvValidLifetime 285
#define T_AdvPreferredLifetime 286
#define T_AdvRouterAddr 287
#define T_AdvHomeAgentFlag 288
#define T_AdvIntervalOpt 289
#define T_AdvHomeAgentInfo 290
#define T_Base6to4Interface 291
#define T_UnicastOnly 292
#define T_EnNp 293
#define T_HomeAgentPreference 294
#define T_HomeAgentLifetime 295
#define T_AdvRoutePreference 296
#define T_AdvRouteLifetime 297
#define T_AdvMapping 298
#define T_AdvRDNSSPreference 299
#define T_AdvRDNSSOpenFlag 300
#define T_AdvRDNSSLifetime 301
#define T_AdvMobRtrSupportFlag 302
#define T_BAD_TOKEN 303
#define mapplist 304
typedef union {
	unsigned int		num;
	int			snum;
	double			dec;
	int			bool;
	struct in6_addr		*addr;
	char			*str;
	struct AdvPrefix	*pinfo;
	struct AdvRoute		*rinfo;
	struct AdvRDNSS		*rdnssinfo;
	struct np_map_conf  *npmc ;
} YYSTYPE;
extern YYSTYPE yylval;
