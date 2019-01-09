#include "../include/tool.h"

FILE* my_fopen(const char *_Filename, const char *_Mode)
{
	FILE*fp;
#ifdef _WIN32
	if (fopen_s(&fp, _Filename, _Mode) != 0)	return (FILE*)NULL;
	else										return fp;
#else
	return (FILE*)fopen(_Filename, _Mode);
#endif
}

int my_sprintf(char* _DstBuf, size_t _DstSize, const char* _Format, ...)
{
	va_list	args;
	int		len;

	va_start(args, _Format);
#ifdef _WIN32
	len = vsprintf_s(_DstBuf, _DstSize, _Format, args);
#else
	len = vsnprintf(_DstBuf, _DstSize, _Format, args);
#endif
	va_end(args);

	return len;
}

#ifdef _WIN32
errno_t my_strcpy(char* _Dst, rsize_t _DstSize, const char* _Src)
{
	return strcpy_s(_Dst, _DstSize, _Src);
}
errno_t my_access(const char* _Filename, int _AccessMode)
{
	return _access_s(_Filename, _AccessMode);
}
#else
char* my_strcpy(char* s1, size_t n, const char* s2)
{
	return strncpy(s1, s2, n - 1);
}
int my_access(const char* _Filename, int _AccessMode)
{
	return access(_Filename, _AccessMode);
}
#endif

char ItoC(int data)
{
	union UC_I {
		unsigned char	c[2];
		short int		data;
	} wk;

	wk.data = data;

#if defined _WIN32 || defined LINUX
	return wk.c[0];
#else
	return wk.c[1];
#endif
}

/*
*Hex is changed into a character type.
*Function name : Hex2UC
*Argument      : char		*ikey
*                unsigned int	ilen
*                unsigned char	*okey
*Return value  : void
*/
void Hex2UC(char *ikey, unsigned int ilen, unsigned char *okey)
{
	int	ii, tmp_i;
	char	tmp_c[BUFSIZ];

	for (ii = 0; ii < (signed int)ilen; ii++) {
		memcpy(tmp_c, &ikey[ii * 2], 2);
		tmp_c[2] = '\0';
#ifdef _WIN32
		sscanf_s(tmp_c, "%x", &tmp_i);
#else
		sscanf(tmp_c, "%x", &tmp_i);
#endif
		okey[ii] = (unsigned char)tmp_i;
	}
}

/*
*Exclusive-OR
*Function name : xor00n
*Argument      : unsigned char	*a1
*                unsigned char	*a2
*                int		n
*                unsigned char	*b
*Return value  : void
*/
void xor00n(unsigned char *a1, unsigned char *a2, int n, unsigned char *b)
{
	int	ii;

	for (ii = 0; ii < n; ii++)
		b[ii] = (unsigned char)(((unsigned short)(a1[ii]) ^ (unsigned short)(a2[ii])) & 0x00ff);
}

/*
*Function name : fromASCIItoEBCDIC
*Argument      : unsigned char	idata
*Return value  : EBCDIC value(unsigned char)
*/
unsigned char fromASCIItoEBCDIC(unsigned char idata)
{
	int		ii;
	unsigned char	odata;

	idata = (unsigned char)toupper((int)idata);

	ii = 0;
	odata = 0x00;
	while (ASCII[ii] != 0xff) {
		if (idata == ASCII[ii]) {
			odata = EBCDIC[ii];
			break;
		}
		ii++;
	}

	return odata;
}

/*
*Function name : fromEBCDICtoASCII
*Argument      : unsigned char	idata
*Return value  : ASCII value(unsigned char)
*/
unsigned char fromEBCDICtoASCII(unsigned char idata)
{
	int		ii;
	unsigned char	odata;

	ii = 0;
	odata = 0x00;
	while (EBCDIC[ii] != 0xff) {
		if (idata == EBCDIC[ii]) {
			odata = ASCII[ii];
			break;
		}
		ii++;
	}

	return odata;
}

/*
*Function name : I2UC
*Argument      : int		data
*                unsigned char	*c
*Return value  : void
*/
void I2UC(int data, unsigned char *c)
{
	union UC_I	wk;

	wk.data = data;

#if defined _WIN32 || defined LINUX
	c[0] = wk.c[3];
	c[1] = wk.c[2];
	c[2] = wk.c[1];
	c[3] = wk.c[0];
#else
	c[0] = wk.c[0];
	c[1] = wk.c[1];
	c[2] = wk.c[2];
	c[3] = wk.c[3];
#endif
}

/*
*Function name : UC2I
*Argument      : unsigned char	*c
*                int		*data
*Return value  : void
*/
void UC2I(unsigned char *c, int *data)
{
	union UC_I	wk;

#if defined _WIN32 || defined LINUX
	wk.c[0] = c[3];
	wk.c[1] = c[2];
	wk.c[2] = c[1];
	wk.c[3] = c[0];
#else
	wk.c[0] = c[0];
	wk.c[1] = c[1];
	wk.c[2] = c[2];
	wk.c[3] = c[3];
#endif

	*data = wk.data;
}

/*
*Function name : S2UC
*Argument      : short		data
*                unsigned char	*c
*Return value  : void
*/
void S2UC(short data, unsigned char *c)
{
	union UC_S	wk;

	wk.data = data;

#if defined _WIN32 || defined LINUX
	c[0] = wk.c[1];
	c[1] = wk.c[0];
#else
	c[0] = wk.c[0];
	c[1] = wk.c[1];
#endif
}

/*
*Function name : UC2S
*Argument      : unsigned char	*c
*                short		*data
*Return value  : void
*/
void UC2S(unsigned char *c, short *data)
{
	union UC_S	wk;

#if defined _WIN32 || defined LINUX
	wk.c[0] = c[1];
	wk.c[1] = c[0];
#else
	wk.c[0] = c[0];
	wk.c[1] = c[1];
#endif

	*data = wk.data;
}

/*
*Universal Time is converted on the Julius day.
*Function name : ut2jd
*Argument      : double	*d
*                int		year
*                int		month
*                int		day
*                int		hour
*                int		min
*                int		sec
*Return value  : Value on Julius day(integer value)
*/
long ut2jd(double *d, int year, int month, int day, int hour, int min, int sec)
{
	int	bc, gregory;
	long	j;

	bc = year <= 0;

	/* First day at Gregory's calendar */
	gregory = year > 1582 || (year == 1582 && month > 10) ||
		(year == 1582 && month == 10 && day >= 15);

	if (month <= 2) {
		year--;
		month += 12;
	}

	if (hour < 12) {
		j = 0;
		*d = 0.5;
	}
	else {
		j = 1;
		*d = -0.5;
	}
	*d += (hour * 3600L + min * 60 + sec) / 86400.0;

	j += bc ? (year - 3) / 4 : year / 4;
	if (gregory) j += 2 - year / 100 + year / 400;
	j += 1720994L + year * 365L + (month + 1) * 30 + (month + 1) * 3 / 5 + day;

	return j;
}

/*
*The Julius day is converted at Universal Time.
*Function name : jd2ut
*Argument      : int		*year
*                int		*month
*                int		*day
*                int		*hour
*                int		*min
*                int		*sec
*                long		j
*                double	d
*Return value  : void
*/
void jd2ut(int *year, int *month, int *day, int *hour, int *min, int *sec, long j, double d)
{
	long c, k, e, s;

	if (d >= 0.5) {
		j++;
		d -= 0.5;
	}
	else d += 0.5;

	if (j >= 2299161) j = j + 1 + (j - 1867216) / 36524 - (j - 1867216) / 146096;
	j += 1524;

	c = (long)((j - 122.1) / 365.25);
	k = 365L * c + c / 4;
	e = (long)((j - k) / 30.6001);

	*year = c - 4716;
	*month = e - 1;
	if (*month > 12) {
		(*month) -= 12;
		(*year)++;
	}
	*day = j - k - (long)(30.6*e);

	s = (long)(d * 86400 + 0.5);
	*hour = s / 3600;
	*min = (s % 3600) / 60;
	*sec = s % 60;
}

/*
*The date after n day is calculated.
*Function name : dayAdd
*Argument      : int	*year2
*                int	*month2
*                int	*day2
*                int	year1
*                int	month1
*                int	day1
*                int	n
*Return value  : void
*/
void dayAdd(int *year2, int *month2, int *day2, int year1, int month1, int day1, int n)
{
	long j;
	double d;
	int hour, min, sec;

	j = ut2jd(&d, year1, month1, day1, 12, 0, 0);
	jd2ut(year2, month2, day2, &hour, &min, &sec, j + n, 0);
}

/*
*Days for two dates are calculated.
*Function name : dayDiff
*Argument      : int	year1
*                int	month1
*                int	day1
*                int	year2
*                int	month2
*                int	day2
*Return value  : Days.
*                If it is a value of the minus, it is shown that the context at the date has reversed.
*/
int dayDiff(int year1, int month1, int day1, int year2, int month2, int day2)
{
	double d;

	return ut2jd(&d, year2, month2, day2, 12, 0, 0) -
		ut2jd(&d, year1, month1, day1, 12, 0, 0);
}

/*
*The age at the Christian era is judged whether it is a leap year.
*Function name : IsLeapYear
*Argument      : int	Year
*Return value  : 1:Leap year
*                0:Not leap year
*/
int IsLeapYear(int Year)
{
	return (((Year % 4) == 0) && ((Year % 100) != 0) || ((Year % 400) == 0));
}

/*
*Days for one month are returned.
*Function name : DaysInMonth
*Argument      : int	nYear
*                int	nMonth
*Return value  : Days.
*/
int DaysInMonth(int nYear, int nMonth)
{
	const int nDaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if ((nMonth < 1) || (nMonth > 12)) return 0;
	if ((2 == nMonth) && IsLeapYear(nYear)) return 29;

	return nDaysInMonth[nMonth - 1];
}

/*
*Whether a specified date is effective is judged.
*Function name : IsGoodDate
*Argument      : int	Year
*                int	Month
*                int	Day
*Return value  :
*/
int IsGoodDate(int Year, int Month, int Day)
{
	/*return ((Month>=1)&&(Month<=12)&&(Day>=1)&&(Day<=DaysInMonth(Year,Month)));*/
	return ((Year >= 1) && (Year <= 9999) && (Month >= 1) && (Month <= 12) && (Day >= 1) && (Day <= 31));
}

/*
*Proc time.
*Function name : proc_time
*Argument      : none
*Return value  : proc time
*/
char* proc_time()
{
#ifdef _WIN32
	__time64_t	longtime;
	struct tm		newtime;
#else
	time_t	longtime;
#endif
	struct tm	*dt;
	static char	time_msg[BUFSIZ];

#ifdef _WIN32
	_time64(&longtime);
	_localtime64_s(&newtime, &longtime);
	dt = &newtime;
#else
	time(&longtime);
	dt = localtime(&longtime);
#endif

	my_sprintf(time_msg, sizeof(time_msg), "%02d:%02d:%02d", dt->tm_hour, dt->tm_min, dt->tm_sec);

	return time_msg;
}

/*
*Decomposition of the character sequence divided in a certain pause character
*Function name : RecPartition
*Argument      : char separate         : Pause character
*                char *buff            : Character sequence
*                int  *data_count      : The number of storing arrangement
*                char data[][NCOLUMNS] : The divided character sequence
*Return value  : Nothing
*/
int RecPartition(char separate, char *buff, int *data_count, char data[][NCOLUMNS])
{
	int	quated = 0, first = 1;
	int	ii, jj, kk;
	int	length;

	length = (int)strlen(buff);
	for (ii = jj = kk = 0; ii < length; ii++) {
		if (buff[ii] == '"')	quated = !quated;
		if (quated) {
			if (buff[ii] != '"' || quated) {
				if (buff[ii] != '"' || !first) {
					if (kk >= NCOLUMNS)	return EXIT_FAILURE;
					data[jj][kk] = buff[ii];
					kk++;
				}
			}
			first = 0;
		}
		else {
			if (buff[ii] == separate) {
				if (jj >= NROWS)	return EXIT_FAILURE;
				if (kk >= NCOLUMNS)	return EXIT_FAILURE;
				data[jj][kk] = '\0';
				jj++;
				kk = 0;
				first = 1;
			}
			else {
				if (buff[ii] != '"' || quated) {
					if (kk >= NCOLUMNS)	return EXIT_FAILURE;
					data[jj][kk] = buff[ii];
					kk++;
				}
				first = 0;
			}
		}
	}
	if (kk >= NCOLUMNS)	return EXIT_FAILURE;
	data[jj][kk] = '\0';

	*data_count = jj + 1;

	return EXIT_SUCCESS;
}

/*
*A path is divided into a directory name and a file name.
*Function name : splitpath
*Argument      : char *path : A path
*                char *dir  : A directory name
*                int  *file : A file name
*Return value  :     0:Normal end
*                other:Unusual end
*/
int splitpath(char *path, char *dir, char *file)
{
	int	ii, pos = 0;

	memset(dir, 0x0, sizeof(dir));
	memset(file, 0x0, sizeof(file));

	if (strlen(path) <= 0)	return 73;

	for (ii = (int)strlen(path) - 1; ii >= 0; ii--) {
		if (path[ii] == '/') {
			pos = ii;
			break;
		}
	}

	if (pos == 0) {
		dir[0] = '.';
		dir[1] = '\0';
		for (ii = 0; ii < (signed int)strlen(path); ii++)
			file[ii] = path[ii];
		file[ii] = '\0';
	}
	else {
		for (ii = 0; ii < pos; ii++)	dir[ii] = path[ii];
		dir[pos] = '\0';
		for (ii = 0; ii < (signed int)strlen(path) - pos - 1; ii++)
			file[ii] = path[pos + ii + 1];
		file[ii] = '\0';
	}

	return 0;
}

/*
*Delete leading spaces and trailing spaces
*Function name : trim
*Argument      : char *s : Words
*Return value  : Nothing
*/
void trim(char *s)
{
	char  *a, *b, *p;

	/*for(a = s; *a == ' '; a++);*/
	a = s;
	for (p = b = a; *p; )  if (*p++ != ' ') b = p;
	while (a != b) *s++ = *a++;
	*s = '\0';
}
void trim_all(char *s)
{
	char  *a, *b, *p;

	for (a = s; *a == ' '; a++);
	for (p = b = a; *p; )  if (*p++ != ' ') b = p;
	while (a != b) *s++ = *a++;
	*s = '\0';
}

#ifndef _WIN32
int CmdPathGet(char cmd1[], char cmd2[])
{
	char	cmd[BUFSIZ];
	char	record[BUFSIZ];
	FILE	*fp;

	cmd2[0] = (char)'\0';

	snprintf(cmd, sizeof(cmd), "which %s", cmd1);
	if ((fp = (FILE *)popen(cmd, "r")) == NULL) {
		fprintf(stderr, "Command error('which')\n");
		return 1;
	}

	if (fgets(record, sizeof(record), fp) == NULL) {
		fprintf(stderr, "Command error('which')\n");
		return 1;
	}
	pclose(fp);

	record[(int)strlen(record) - 1] = (char)'\0';
	if (memcmp(record, "no ", 3) == 0) {
		fprintf(stderr, "Command not found\n");
		return 1;
	}

	strcpy(cmd2, record);

	return 0;
}

int ExecPathGet(char cmd[], char path[])
{
	int	i;

	if (CmdPathGet(cmd, path) != 0) {
		strcpy(path, cmd);
	}

	for (i = (int)strlen(path) - 1; i >= 0; i--) {
		if (path[i] == '/') {
			path[i] = (char)'\0';
			break;
		}
		path[i] = (char)'\0';
	}
	if ((int)strlen(path) == 0) {
		strcpy(path, ".");
	}

	return 0;
}
#endif
