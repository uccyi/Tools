#ifndef CONFIG_H
#define CONFIG_H

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<memory.h>

/*
 * Configuration File Get Program.
 *
 * [section-1]
 * item1-1	= value1-1		# comment for value1-1.
 *					# plural lines's example.
 * item1-2	= value1-2
 * [section-2]
 * item2-1	= value2-1
 * item2-2	= value2-2
 * 
 */

#define		CONFIG_BUF_SIZE		1024
#define		COMMENT_STR		("#")
#define		COMMENT_BLK		(" ")
#define		COMMENT_TAB		("\t")
#define		COMMENT_RET		("\n")

#define		RESERVE			("SETENV")	/* Reservation word */

typedef	struct{
    char*	lstr;		/* Left character sequence	*/
    char*	rstr;		/* Right character sequence	*/
}_CONFIG_VALUE;

/* The character sequence for every section */
typedef	struct{
    char*	section;	/* Section name	*/
    int		cnt;		/* The number of setting character sequences	*/
    _CONFIG_VALUE*	value;	/* Setting character sequence	*/
}_CONFIG_BUFFER;
extern	int		configuration_counter;
extern	_CONFIG_BUFFER	*configuration;

/* EXTERN function */

/* Input and output */
extern	int	ConfigOpen(const char*);
extern	void	ConfigClose(void);
extern	void	ConfigConstructor();

/* Memory reservation */
extern	int	ConfigRead(void);
extern	int	is_comment_line(char*);
extern	int	configuration_alloc(void);
extern	int	value_alloc(_CONFIG_BUFFER*);

/* Acquisition */
extern	char*	ConfigGet(const char*, const char*);
extern	int	value_proc(char*);
extern	int	value_lstr_set(_CONFIG_BUFFER*, char*);
extern	int	value_rstr_set(_CONFIG_BUFFER*, char*);
extern	void	environment_development(char*, int*);

/* Tool */
extern	int	strpos(char*, const char*);
extern	char*	rstrtok(char*);
extern	int	section_proc(char*);

#endif /* CONFIG_H */
