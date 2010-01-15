#include <stdlib.h>
#ifndef lint
//#ifdef __unused
//__unused
//#endif
static char const 
yyrcsid[] = "$FreeBSD: src/usr.bin/yacc/skeleton.c,v 1.37 2003/02/12 18:03:55 davidc Exp $";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYLEX yylex()
#define YYEMPTY -1
#define yyclearin (yychar=(YYEMPTY))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING() (yyerrflag!=0)
#if defined(__cplusplus) || __STDC__
static int yygrowstack(void);
#else
static int yygrowstack();
#endif
#define YYPREFIX "yy"
#line 17 "gram.y"
#include <config.h>
#include <includes.h>
#include <radvd.h>
#include <defaults.h>

extern struct Interface *IfaceList;
struct Interface *iface = NULL;
struct AdvPrefix *prefix = NULL;
struct AdvRoute *route = NULL;
struct AdvRDNSS *rdnss = NULL;
struct np_map_conf *npm = NULL;

extern char *conf_file;
extern int num_lines;
extern char *yytext;
extern int sock;

static void cleanup(void);
static void yyerror(char *msg);

#if 0 /* no longer necessary? */
#ifndef HAVE_IN6_ADDR_S6_ADDR
# ifdef __FreeBSD__
#  define s6_addr32 __u6_addr.__u6_addr32
#  define s6_addr16 __u6_addr.__u6_addr16
# endif
#endif
#endif

#define ABORT	do { cleanup(); YYABORT; } while (0);

#line 120 "gram.y"
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
#line 69 "gram.c"
#define YYERRCODE 256
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
const short yylhs[] = {                                        -1,
    0,    0,   15,   16,    1,   17,   18,   18,    2,    2,
    5,    5,   12,   12,    8,    8,   19,   19,   20,   20,
   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,
   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,
   20,   20,   20,    4,    3,   21,   22,   22,   23,   23,
   24,   24,   24,   24,   24,   24,    7,    6,   25,   26,
   26,   27,   27,   28,   28,   14,   13,   29,   30,   30,
   31,   10,    9,   34,   34,   35,   32,   33,   33,   36,
   36,   37,   37,   37,   11,   11,
};
const short yylen[] = {                                         2,
    2,    1,    5,    2,    1,    5,    0,    1,    0,    1,
    0,    1,    0,    1,    0,    1,    2,    1,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    2,    5,    4,    0,    1,    2,    1,
    3,    3,    3,    3,    3,    3,    2,    5,    4,    0,
    1,    2,    1,    3,    3,    2,    5,    4,    2,    1,
    3,    2,    5,    2,    1,    1,    2,    0,    1,    2,
    1,    3,    3,    3,    1,    1,
};
const short yydefred[] = {                                      0,
    0,    0,    2,    0,    5,    4,    1,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   18,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   10,    0,   17,   25,   26,   20,   23,
   19,   22,   21,   24,   27,   28,   29,   30,   31,   34,
   32,   33,   35,   38,   36,   37,   41,   43,   39,   40,
   42,    3,    0,    0,    0,    0,   12,    0,   44,    0,
    0,    0,    0,    0,    0,   14,    0,   57,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   50,   46,    0,
    0,    0,    6,    0,   16,    0,   66,    0,    0,    0,
    0,    0,   63,    0,    0,   85,   86,    0,    0,    0,
    0,    0,   49,   59,    0,   76,    0,   75,   72,    0,
    0,    0,    0,   70,    0,    0,    0,   62,   51,   52,
   54,   55,   53,   56,   45,   68,   74,    0,    0,    0,
    0,    0,   81,    0,   69,    0,   64,   65,   58,    0,
    0,    0,    0,   80,   71,   67,   82,   83,   84,   73,
};
const short yydgoto[] = {                                       2,
    6,   62,   63,   64,   95,   96,   97,  123,  124,  125,
  138,  104,  105,  106,    3,    4,   31,   32,   33,   34,
   65,  116,  117,  118,   98,  131,  132,  133,  107,  153,
  154,  126,  171,  147,  148,  172,  173,
};
const short yysindex[] = {                                   -254,
 -256, -254,    0, -107,    0,    0,    0, -250, -225, -215,
 -248, -230, -229, -213, -209, -226, -204, -203, -202, -201,
 -200, -199, -198, -197, -196, -195, -194, -190, -189, -191,
  -62, -193, -250,    0,    7,   17,   18,   19,   20,   21,
   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
   32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 -166, -157, -193,    0,  -20,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   57, -162, -155, -157,    0,  -16,    0, -237,
 -154,   61, -153, -149, -155,    0,  -11,    0, -241, -152,
 -151, -258, -258, -150, -256,  -12, -237,    0,    0, -146,
   71, -148,    0, -149,    0,   -3,    0, -296, -143, -258,
   -2, -241,    0,   63,   65,    0,    0,   66,   67,   68,
   69,   70,    0,    0, -133,    0, -148,    0,    0, -287,
 -135, -147,   42,    0,   73,   88,   89,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -113, -117, -258,
   43, -287,    0,   93,    0,   94,    0,    0,    0,   95,
   96,   97,   98,    0,    0,    0,    0,    0,    0,    0,
};
const short yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0, -125,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -118, -121,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -116, -118,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -114, -116,    0,    0,    0,   44,
    0,    0,    0,   45, -114,    0,    0,    0,  -62,    0,
    0,    0,    0,    0,    0,    0,   46,    0,    0,    0,
    0,    0,    0,   45,    0,    0,    0,    0,    0,    0,
    0,   47,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   50,    0,    0,   49,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   51,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
const short yygindex[] = {                                      0,
   48,   99,    0,    0,   62,    0,    0,   53,    0,    0,
 -112,   54,    0,    0,  158,    0,    0,    0,    0,  128,
    0,    0,    0,   58,    0,    0,    0,   52,    0,    0,
   12,    0,    0,    0,   55,    0,   -7,
};
#define YYTABLESIZE 202
const short yytable[] = {                                       7,
  139,  151,    1,    8,  136,    5,    9,  152,   11,  137,
   13,  168,  169,  170,   37,    8,   38,  156,    9,   10,
   11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
   21,   22,   39,   41,   40,   42,   45,   23,   24,   25,
   35,   26,   27,   28,   29,  110,  111,  112,  113,  114,
   36,   30,   43,  115,  129,  130,   44,  182,   46,   47,
   48,   49,   60,   50,   61,   67,   51,   52,   53,   54,
   55,   56,   57,   58,   59,   68,   69,   70,   71,   72,
   73,   74,   75,   76,   77,   78,   79,   80,   81,   82,
   83,   84,   85,   86,   87,   88,   89,   90,   91,   92,
   93,   94,  100,  101,  102,  103,  109,  120,  119,  121,
  122,  128,  142,  134,  135,  140,  144,  145,  146,  150,
  155,  159,  157,  160,  161,  162,  163,  164,  165,  166,
  174,  177,    7,    7,    7,    7,    8,    8,    8,    8,
    9,    9,    9,   11,   11,   13,  178,  179,  181,  180,
  151,  185,  186,  187,  188,  189,  190,  108,  127,    7,
   66,   99,  141,  175,  184,    0,  176,  183,   47,   15,
   48,   61,   77,   78,  143,   79,  149,    0,    0,    0,
    0,    0,    0,  158,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  167,
};
const short yycheck[] = {                                     125,
  113,  298,  257,  125,  263,  262,  125,  304,  125,  268,
  125,  299,  300,  301,  263,  123,  265,  130,  269,  270,
  271,  272,  273,  274,  275,  276,  277,  278,  279,  280,
  281,  282,  263,  263,  265,  265,  263,  288,  289,  290,
  266,  292,  293,  294,  295,  283,  284,  285,  286,  287,
  266,  302,  266,  291,  296,  297,  266,  170,  263,  263,
  263,  263,  125,  264,  258,   59,  266,  266,  266,  266,
  266,  266,  263,  263,  266,   59,   59,   59,   59,   59,
   59,   59,   59,   59,   59,   59,   59,   59,   59,   59,
   59,   59,   59,   59,   59,   59,   59,   59,   59,   59,
  267,  259,  123,   47,  267,  261,  123,   47,  263,  263,
  260,  123,  125,  266,  266,  266,  263,   47,  267,  123,
  264,   59,  125,   59,   59,   59,   59,   59,   59,  263,
  266,   59,  258,  259,  260,  261,  258,  259,  260,  261,
  259,  260,  261,  260,  261,  260,   59,   59,  266,  263,
  298,   59,   59,   59,   59,   59,   59,   96,  105,    2,
   33,   63,  115,  152,  172,   -1,  125,  125,  125,  125,
  125,  125,  123,  125,  117,  125,  124,   -1,   -1,   -1,
   -1,   -1,   -1,  132,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  147,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 304
#if YYDEBUG
const char * const yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,"'/'",0,0,0,0,0,0,0,0,0,0,0,"';'",0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"T_INTERFACE",
"T_PREFIX","T_ROUTE","T_RDNSS","T_NPMAP","STRING","NUMBER","SIGNEDNUMBER",
"DECIMAL","SWITCH","IPV6ADDR","INFINITY","T_IgnoreIfMissing","T_AdvSendAdvert",
"T_MaxRtrAdvInterval","T_MinRtrAdvInterval","T_MinDelayBetweenRAs",
"T_AdvManagedFlag","T_AdvOtherConfigFlag","T_AdvLinkMTU","T_AdvReachableTime",
"T_AdvRetransTimer","T_AdvCurHopLimit","T_AdvDefaultLifetime",
"T_AdvDefaultPreference","T_AdvSourceLLAddress","T_AdvOnLink","T_AdvAutonomous",
"T_AdvValidLifetime","T_AdvPreferredLifetime","T_AdvRouterAddr",
"T_AdvHomeAgentFlag","T_AdvIntervalOpt","T_AdvHomeAgentInfo",
"T_Base6to4Interface","T_UnicastOnly","T_EnNp","T_HomeAgentPreference",
"T_HomeAgentLifetime","T_AdvRoutePreference","T_AdvRouteLifetime",
"T_AdvMapping","T_AdvRDNSSPreference","T_AdvRDNSSOpenFlag","T_AdvRDNSSLifetime",
"T_AdvMobRtrSupportFlag","T_BAD_TOKEN","mapplist",
};
const char * const yyrule[] = {
"$accept : grammar",
"grammar : grammar ifacedef",
"grammar : ifacedef",
"ifacedef : ifacehead '{' ifaceparams '}' ';'",
"ifacehead : T_INTERFACE name",
"name : STRING",
"ifaceparams : optional_ifacevlist optional_prefixlist optional_routelist optional_maplist optional_rdnsslist",
"optional_ifacevlist :",
"optional_ifacevlist : ifacevlist",
"optional_prefixlist :",
"optional_prefixlist : prefixlist",
"optional_routelist :",
"optional_routelist : routelist",
"optional_maplist :",
"optional_maplist : maplist",
"optional_rdnsslist :",
"optional_rdnsslist : rdnsslist",
"ifacevlist : ifacevlist ifaceval",
"ifacevlist : ifaceval",
"ifaceval : T_MinRtrAdvInterval NUMBER ';'",
"ifaceval : T_MaxRtrAdvInterval NUMBER ';'",
"ifaceval : T_MinDelayBetweenRAs NUMBER ';'",
"ifaceval : T_MinRtrAdvInterval DECIMAL ';'",
"ifaceval : T_MaxRtrAdvInterval DECIMAL ';'",
"ifaceval : T_MinDelayBetweenRAs DECIMAL ';'",
"ifaceval : T_IgnoreIfMissing SWITCH ';'",
"ifaceval : T_AdvSendAdvert SWITCH ';'",
"ifaceval : T_AdvManagedFlag SWITCH ';'",
"ifaceval : T_AdvOtherConfigFlag SWITCH ';'",
"ifaceval : T_AdvLinkMTU NUMBER ';'",
"ifaceval : T_AdvReachableTime NUMBER ';'",
"ifaceval : T_AdvRetransTimer NUMBER ';'",
"ifaceval : T_AdvDefaultLifetime NUMBER ';'",
"ifaceval : T_AdvDefaultPreference SIGNEDNUMBER ';'",
"ifaceval : T_AdvCurHopLimit NUMBER ';'",
"ifaceval : T_AdvSourceLLAddress SWITCH ';'",
"ifaceval : T_AdvIntervalOpt SWITCH ';'",
"ifaceval : T_AdvHomeAgentInfo SWITCH ';'",
"ifaceval : T_AdvHomeAgentFlag SWITCH ';'",
"ifaceval : T_HomeAgentPreference NUMBER ';'",
"ifaceval : T_HomeAgentLifetime NUMBER ';'",
"ifaceval : T_UnicastOnly SWITCH ';'",
"ifaceval : T_AdvMobRtrSupportFlag SWITCH ';'",
"ifaceval : T_EnNp SWITCH ';'",
"prefixlist : prefixdef optional_prefixlist",
"prefixdef : prefixhead '{' optional_prefixplist '}' ';'",
"prefixhead : T_PREFIX IPV6ADDR '/' NUMBER",
"optional_prefixplist :",
"optional_prefixplist : prefixplist",
"prefixplist : prefixplist prefixparms",
"prefixplist : prefixparms",
"prefixparms : T_AdvOnLink SWITCH ';'",
"prefixparms : T_AdvAutonomous SWITCH ';'",
"prefixparms : T_AdvRouterAddr SWITCH ';'",
"prefixparms : T_AdvValidLifetime number_or_infinity ';'",
"prefixparms : T_AdvPreferredLifetime number_or_infinity ';'",
"prefixparms : T_Base6to4Interface name ';'",
"routelist : routedef optional_routelist",
"routedef : routehead '{' optional_routeplist '}' ';'",
"routehead : T_ROUTE IPV6ADDR '/' NUMBER",
"optional_routeplist :",
"optional_routeplist : routeplist",
"routeplist : routeplist routeparms",
"routeplist : routeparms",
"routeparms : T_AdvRoutePreference SIGNEDNUMBER ';'",
"routeparms : T_AdvRouteLifetime number_or_infinity ';'",
"maplist : mapdef optional_maplist",
"mapdef : maphead '{' optional_mapplist '}' ';'",
"maphead : T_NPMAP NUMBER '/' NUMBER",
"optional_mapplist : mapplist mapparms",
"optional_mapplist : mapparms",
"mapparms : T_AdvMapping SWITCH ';'",
"rdnsslist : rdnssdef optional_rdnsslist",
"rdnssdef : rdnsshead '{' optional_rdnssplist '}' ';'",
"rdnssaddrs : rdnssaddrs rdnssaddr",
"rdnssaddrs : rdnssaddr",
"rdnssaddr : IPV6ADDR",
"rdnsshead : T_RDNSS rdnssaddrs",
"optional_rdnssplist :",
"optional_rdnssplist : rdnssplist",
"rdnssplist : rdnssplist rdnssparms",
"rdnssplist : rdnssparms",
"rdnssparms : T_AdvRDNSSPreference NUMBER ';'",
"rdnssparms : T_AdvRDNSSOpenFlag SWITCH ';'",
"rdnssparms : T_AdvRDNSSLifetime number_or_infinity ';'",
"number_or_infinity : NUMBER",
"number_or_infinity : INFINITY",
};
#endif
#if YYDEBUG
#include <stdio.h>
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH 10000
#endif
#endif
#define YYINITSTACKSIZE 200
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short *yyss;
short *yysslim;
YYSTYPE *yyvs;
int yystacksize;
#line 697 "gram.y"

static
void cleanup(void)
{
	if (iface)
		free(iface);
	
	if (prefix)
		free(prefix);

	if (route)
		free(route);

	if (rdnss)
		free(rdnss);
}

static void
yyerror(char *msg)
{
	cleanup();
	flog(LOG_ERR, "%s in %s, line %d: %s", msg, conf_file, num_lines, yytext);
}
#line 431 "gram.c"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack()
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;
    i = yyssp - yyss;
    newss = yyss ? (short *)realloc(yyss, newsize * sizeof *newss) :
      (short *)malloc(newsize * sizeof *newss);
    if (newss == NULL)
        return -1;
    yyss = newss;
    yyssp = newss + i;
    newvs = yyvs ? (YYSTYPE *)realloc(yyvs, newsize * sizeof *newvs) :
      (YYSTYPE *)malloc(newsize * sizeof *newvs);
    if (newvs == NULL)
        return -1;
    yyvs = newvs;
    yyvsp = newvs + i;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab

#ifndef YYPARSE_PARAM
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG void
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif	/* ANSI-C/C++ */
#else	/* YYPARSE_PARAM */
#ifndef YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM_TYPE void *
#endif
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG YYPARSE_PARAM_TYPE YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL YYPARSE_PARAM_TYPE YYPARSE_PARAM;
#endif	/* ANSI-C/C++ */
#endif	/* ! YYPARSE_PARAM */

int
yyparse (YYPARSE_PARAM_ARG)
    YYPARSE_PARAM_DECL
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate])) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#if defined(lint) || defined(__GNUC__)
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#if defined(lint) || defined(__GNUC__)
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 3:
#line 140 "gram.y"
{
			struct Interface *iface2;

			iface2 = IfaceList;
			while (iface2)
			{
				if (!strcmp(iface2->Name, iface->Name))
				{
					flog(LOG_ERR, "duplicate interface "
						"definition for %s", iface->Name);
					ABORT;
				}
				iface2 = iface2->next;
			}			

			if (check_device(sock, iface) < 0) {
				if (iface->IgnoreIfMissing) {
					dlog(LOG_DEBUG, 4, "interface %s did not exist, ignoring the interface", iface->Name);
					goto skip_interface;
				}
				else {
					flog(LOG_ERR, "interface %s does not exist", iface->Name);
					ABORT;
				}
			}
			if (setup_deviceinfo(sock, iface) < 0)
				ABORT;
			if (check_iface(iface) < 0)
				ABORT;
			if (setup_linklocal_addr(sock, iface) < 0)
				ABORT;
			if (setup_allrouters_membership(sock, iface) < 0)
				ABORT;

			iface->next = IfaceList;
			IfaceList = iface;

			dlog(LOG_DEBUG, 4, "interface definition for %s is ok", iface->Name);

skip_interface:
			iface = NULL;
		}
break;
case 4:
#line 184 "gram.y"
{
			iface = malloc(sizeof(struct Interface));

			if (iface == NULL) {
				flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
				ABORT;
			}

			iface_init_defaults(iface);
			strncpy(iface->Name, yyvsp[0].str, IFNAMSIZ-1);
			iface->Name[IFNAMSIZ-1] = '\0';
		}
break;
case 5:
#line 199 "gram.y"
{
			/* check vality */
			yyval.str = yyvsp[0].str;
		}
break;
case 6:
#line 206 "gram.y"
{
			iface->AdvPrefixList = yyvsp[-3].pinfo;
			iface->AdvRouteList = yyvsp[-2].rinfo;
			iface->NpMapList = yyvsp[-1].npmc;
			iface->AdvRDNSSList = yyvsp[0].rdnssinfo;
		}
break;
case 9:
#line 219 "gram.y"
{
			yyval.pinfo = NULL;
		}
break;
case 11:
#line 226 "gram.y"
{
			yyval.rinfo = NULL;
		}
break;
case 13:
#line 233 "gram.y"
{
			yyval.npmc = NULL;
		}
break;
case 15:
#line 240 "gram.y"
{
			yyval.rdnssinfo = NULL;
		}
break;
case 19:
#line 251 "gram.y"
{
			iface->MinRtrAdvInterval = yyvsp[-1].num;
		}
break;
case 20:
#line 255 "gram.y"
{
			iface->MaxRtrAdvInterval = yyvsp[-1].num;
		}
break;
case 21:
#line 259 "gram.y"
{
			iface->MinDelayBetweenRAs = yyvsp[-1].num;
		}
break;
case 22:
#line 263 "gram.y"
{
			iface->MinRtrAdvInterval = yyvsp[-1].dec;
		}
break;
case 23:
#line 267 "gram.y"
{
			iface->MaxRtrAdvInterval = yyvsp[-1].dec;
		}
break;
case 24:
#line 271 "gram.y"
{
			iface->MinDelayBetweenRAs = yyvsp[-1].dec;
		}
break;
case 25:
#line 275 "gram.y"
{
			iface->IgnoreIfMissing = yyvsp[-1].bool;
		}
break;
case 26:
#line 279 "gram.y"
{
			iface->AdvSendAdvert = yyvsp[-1].bool;
		}
break;
case 27:
#line 283 "gram.y"
{
			iface->AdvManagedFlag = yyvsp[-1].bool;
		}
break;
case 28:
#line 287 "gram.y"
{
			iface->AdvOtherConfigFlag = yyvsp[-1].bool;
		}
break;
case 29:
#line 291 "gram.y"
{
			iface->AdvLinkMTU = yyvsp[-1].num;
		}
break;
case 30:
#line 295 "gram.y"
{
			iface->AdvReachableTime = yyvsp[-1].num;
		}
break;
case 31:
#line 299 "gram.y"
{
			iface->AdvRetransTimer = yyvsp[-1].num;
		}
break;
case 32:
#line 303 "gram.y"
{
			iface->AdvDefaultLifetime = yyvsp[-1].num;
		}
break;
case 33:
#line 307 "gram.y"
{
			iface->AdvDefaultPreference = yyvsp[-1].snum;
		}
break;
case 34:
#line 311 "gram.y"
{
			iface->AdvCurHopLimit = yyvsp[-1].num;
		}
break;
case 35:
#line 315 "gram.y"
{
			iface->AdvSourceLLAddress = yyvsp[-1].bool;
		}
break;
case 36:
#line 319 "gram.y"
{
			iface->AdvIntervalOpt = yyvsp[-1].bool;
		}
break;
case 37:
#line 323 "gram.y"
{
			iface->AdvHomeAgentInfo = yyvsp[-1].bool;
		}
break;
case 38:
#line 327 "gram.y"
{
			iface->AdvHomeAgentFlag = yyvsp[-1].bool;
		}
break;
case 39:
#line 331 "gram.y"
{
			iface->HomeAgentPreference = yyvsp[-1].num;
		}
break;
case 40:
#line 335 "gram.y"
{
			iface->HomeAgentLifetime = yyvsp[-1].num;
		}
break;
case 41:
#line 339 "gram.y"
{
			iface->UnicastOnly = yyvsp[-1].bool;
		}
break;
case 42:
#line 343 "gram.y"
{
			iface->AdvMobRtrSupportFlag = yyvsp[-1].bool;
		}
break;
case 43:
#line 347 "gram.y"
{
			iface->EnNp = yyvsp[-1].bool;
		}
break;
case 44:
#line 362 "gram.y"
{
			yyvsp[-1].pinfo->next = yyvsp[0].pinfo;
			yyval.pinfo = yyvsp[-1].pinfo;
		}
break;
case 45:
#line 369 "gram.y"
{
			unsigned int dst;

			if (prefix->AdvPreferredLifetime >
			    prefix->AdvValidLifetime)
			{
				flog(LOG_ERR, "AdvValidLifeTime must be "
					"greater than AdvPreferredLifetime in %s, line %d", 
					conf_file, num_lines);
				ABORT;
			}

			if( prefix->if6to4[0] )
			{
				if (get_v4addr(prefix->if6to4, &dst) < 0)
				{
					flog(LOG_ERR, "interface %s has no IPv4 addresses, disabling 6to4 prefix", prefix->if6to4 );
					prefix->enabled = 0;
				} else
				{
					*((uint16_t *)(prefix->Prefix.s6_addr)) = htons(0x2002);
					memcpy( prefix->Prefix.s6_addr + 2, &dst, sizeof( dst ) );
				}
			}

			yyval.pinfo = prefix;
			prefix = NULL;
		}
break;
case 46:
#line 400 "gram.y"
{
			struct in6_addr zeroaddr;
			prefix = malloc(sizeof(struct AdvPrefix));
			
			if (prefix == NULL) {
				flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
				ABORT;
			}

			prefix_init_defaults(prefix);

			if (yyvsp[0].num > MAX_PrefixLen)
			{
				flog(LOG_ERR, "invalid prefix length in %s, line %d", conf_file, num_lines);
				ABORT;
			}

			prefix->PrefixLen = yyvsp[0].num;

			memcpy(&prefix->Prefix, yyvsp[-2].addr, sizeof(struct in6_addr));

			memset(&zeroaddr, 0, sizeof(zeroaddr));
			if (!memcmp(yyvsp[-2].addr, &zeroaddr, sizeof(struct in6_addr))) {
#ifndef HAVE_IFADDRS_H
				flog(LOG_ERR, "invalid all-zeros prefix in %s, line %d", conf_file, num_lines);
				ABORT;
#else
				dlog(LOG_DEBUG, 5, "all-zeros prefix in %s, line %d, parsing..", conf_file, num_lines);
				struct ifaddrs *ifap, *ifa;
				if (getifaddrs(&ifap) != 0)
					flog(LOG_ERR, "getifaddrs failed: %s", strerror(errno));
			        for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
				        struct sockaddr_in6 *s6;
					char buf[INET6_ADDRSTRLEN];
					if (strncmp(ifa->ifa_name, iface->Name, IFNAMSIZ))
						continue;
                			if (ifa->ifa_addr->sa_family != AF_INET6)
			                        continue;
					s6 = (struct sockaddr_in6 *)(ifa->ifa_addr);
	                		if (IN6_IS_ADDR_LINKLOCAL(&s6->sin6_addr))
						continue;
					if (inet_ntop(ifa->ifa_addr->sa_family, (void *)&(s6->sin6_addr), buf, sizeof(buf)) == NULL) {
						flog(LOG_ERR, "%s: inet_ntop failed in %s, line %d!", ifa->ifa_name, conf_file, num_lines);
					}
					else {
						dlog(LOG_DEBUG, 5, "auto-selected prefix %s on interface %s", buf, ifa->ifa_name);
						memcpy(&prefix->Prefix, &s6->sin6_addr, sizeof(struct in6_addr));
						prefix->AdvRouterAddr=1;
						prefix->AutoSelected=1;
					}
				}
				if (!memcmp(&prefix->Prefix, &zeroaddr, sizeof(struct in6_addr))) {
					prefix->enabled = 0;
					flog(LOG_WARNING, "no auto-selected prefix on interface %s, disabling advertisements",  iface->Name);
				}
				freeifaddrs(ifap);
				freeifaddrs(ifa);
#endif /* ifndef HAVE_IFADDRS_H */
			}
		}
break;
case 51:
#line 471 "gram.y"
{
			prefix->AdvOnLinkFlag = yyvsp[-1].bool;
		}
break;
case 52:
#line 475 "gram.y"
{
			prefix->AdvAutonomousFlag = yyvsp[-1].bool;
		}
break;
case 53:
#line 479 "gram.y"
{
			if (prefix->AutoSelected && yyvsp[-1].bool == 0)
				flog(LOG_WARNING, "prefix automatically selected, AdvRouterAddr always enabled, ignoring config line %d", num_lines);
			else  
				prefix->AdvRouterAddr = yyvsp[-1].bool;
		}
break;
case 54:
#line 486 "gram.y"
{
			prefix->AdvValidLifetime = yyvsp[-1].num;
		}
break;
case 55:
#line 490 "gram.y"
{
			prefix->AdvPreferredLifetime = yyvsp[-1].num;
		}
break;
case 56:
#line 494 "gram.y"
{
			if (prefix->AutoSelected) {
				flog(LOG_ERR, "automatically selecting the prefix and Base6to4Interface are mutually exclusive");
				ABORT;
			} /* fallthrough */
			dlog(LOG_DEBUG, 4, "using interface %s for 6to4", yyvsp[-1].str);
			strncpy(prefix->if6to4, yyvsp[-1].str, IFNAMSIZ-1);
			prefix->if6to4[IFNAMSIZ-1] = '\0';
		}
break;
case 57:
#line 506 "gram.y"
{
			yyvsp[-1].rinfo->next = yyvsp[0].rinfo;
			yyval.rinfo = yyvsp[-1].rinfo;
		}
break;
case 58:
#line 513 "gram.y"
{
			yyval.rinfo = route;
			route = NULL;
		}
break;
case 59:
#line 520 "gram.y"
{
			route = malloc(sizeof(struct AdvRoute));
			
			if (route == NULL) {
				flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
				ABORT;
			}

			route_init_defaults(route, iface);

			if (yyvsp[0].num > MAX_PrefixLen)
			{
				flog(LOG_ERR, "invalid route prefix length in %s, line %d", conf_file, num_lines);
				ABORT;
			}

			route->PrefixLen = yyvsp[0].num;

			memcpy(&route->Prefix, yyvsp[-2].addr, sizeof(struct in6_addr));
		}
break;
case 64:
#line 552 "gram.y"
{
			route->AdvRoutePreference = yyvsp[-1].snum;
		}
break;
case 65:
#line 556 "gram.y"
{
			route->AdvRouteLifetime = yyvsp[-1].num;
		}
break;
case 66:
#line 562 "gram.y"
{
			yyvsp[-1].npmc->next = yyvsp[0].npmc;
            yyval.npmc = yyvsp[-1].npmc;
		}
break;
case 67:
#line 569 "gram.y"
{
			yyval.npmc = npm;
            npm = NULL;
		}
break;
case 68:
#line 576 "gram.y"
{
			npm = malloc(sizeof(struct np_map_conf)) ;
			npm->mapping = yyvsp[-2].num;
			npm->prio = yyvsp[0].num;
			
		}
break;
case 71:
#line 589 "gram.y"
{
			npm->adv = yyvsp[-1].bool;
		}
break;
case 72:
#line 595 "gram.y"
{
			yyvsp[-1].rdnssinfo->next = yyvsp[0].rdnssinfo;
			yyval.rdnssinfo = yyvsp[-1].rdnssinfo;
		}
break;
case 73:
#line 602 "gram.y"
{
			yyval.rdnssinfo = rdnss;
			rdnss = NULL;
		}
break;
case 76:
#line 613 "gram.y"
{
			if (!rdnss) {
				/* first IP found */
				rdnss = malloc(sizeof(struct AdvRDNSS));
				
				if (rdnss == NULL) {
					flog(LOG_CRIT, "malloc failed: %s", strerror(errno));
					ABORT;
				}

				rdnss_init_defaults(rdnss, iface);
			}
			
			switch (rdnss->AdvRDNSSNumber) {
				case 0:
					memcpy(&rdnss->AdvRDNSSAddr1, yyvsp[0].addr, sizeof(struct in6_addr));
					rdnss->AdvRDNSSNumber++;
					break;
				case 1:
					memcpy(&rdnss->AdvRDNSSAddr2, yyvsp[0].addr, sizeof(struct in6_addr));
					rdnss->AdvRDNSSNumber++;
					break;
				case 2:
					memcpy(&rdnss->AdvRDNSSAddr3, yyvsp[0].addr, sizeof(struct in6_addr));
					rdnss->AdvRDNSSNumber++;
					break;
				default:
					flog(LOG_CRIT, "Too many addresses in RDNSS section");
					ABORT;
			}
			
		}
break;
case 77:
#line 648 "gram.y"
{
			if (!rdnss) {
				flog(LOG_CRIT, "No address specified in RDNSS section");
				ABORT;
			}
		}
break;
case 82:
#line 666 "gram.y"
{
			rdnss->AdvRDNSSPreference = yyvsp[-1].num;
		}
break;
case 83:
#line 670 "gram.y"
{
			rdnss->AdvRDNSSOpenFlag = yyvsp[-1].bool;
		}
break;
case 84:
#line 674 "gram.y"
{
			if (yyvsp[-1].num < iface->MaxRtrAdvInterval && yyvsp[-1].num != 0) {
				flog(LOG_ERR, "AdvRDNSSLifetime must be at least MaxRtrAdvInterval");
				ABORT;
			}
			if (yyvsp[-1].num > 2*(iface->MaxRtrAdvInterval))
				flog(LOG_WARNING, "Warning: AdvRDNSSLifetime <= 2*MaxRtrAdvInterval would allow stale DNS servers to be deleted faster");

			rdnss->AdvRDNSSLifetime = yyvsp[-1].num;
		}
break;
case 85:
#line 687 "gram.y"
{
                                yyval.num = yyvsp[0].num; 
                        }
break;
case 86:
#line 691 "gram.y"
{
                                yyval.num = (uint32_t)~0;
                        }
break;
#line 1195 "gram.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
