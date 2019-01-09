#include "../include/config.h"

/*
 * config.c  --  This is a configuration creater program.
 */

int configuration_counter;
_CONFIG_BUFFER *configuration;

static FILE *fp = NULL;

int ConfigOpen(const char *filename)
{
  if ((fp = fopen(filename, "r")) == NULL)
    return EXIT_FAILURE;
  ConfigConstructor();
  if (ConfigRead() == EXIT_FAILURE)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

void ConfigClose(void)
{
  int i, j;

  if (fp)
    fclose(fp);

  for (i = 0; i < configuration_counter; i++)
  {
    if (configuration[i].section)
    {
      free(configuration[i].section);
      configuration[i].section = NULL;
    }
    for (j = 0; j < configuration[i].cnt; j++)
    {
      if (configuration[i].value[j].lstr)
      {
        free(configuration[i].value[j].lstr);
        configuration[i].value[j].lstr = NULL;
      }
      if (configuration[i].value[j].rstr)
      {
        free(configuration[i].value[j].rstr);
        configuration[i].value[j].rstr = NULL;
      }
    }
    if (configuration[i].value)
    {
      free(configuration[i].value);
      configuration[i].value = NULL;
    }
  }
  if (configuration)
  {
    free(configuration);
    configuration = NULL;
  }
}

void ConfigConstructor()
{
  configuration_counter = 0;
  configuration = NULL;
}

int ConfigRead(void)
{
  char buf[CONFIG_BUF_SIZE];
  char *str;

  while (fgets(buf, CONFIG_BUF_SIZE, fp) != NULL)
  {
    str = rstrtok(buf);
    if (is_comment_line(str) == 0)
      continue;

    if (str[0] == '[')
    {
      if (section_proc(str) == EXIT_FAILURE)
        return EXIT_FAILURE;
    }
    else
    {
      if (configuration_counter == 0)
        continue;
      if (value_proc(str) == EXIT_FAILURE)
        return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

int is_comment_line(char *str)
{
  if (str == NULL || str[0] == COMMENT_STR[0])
    return 0;

  return 1;
}

char *rstrtok(char *str)
{
  if (str[0] == COMMENT_RET[0])
    return NULL;

  while (str[0] == COMMENT_BLK[0] || str[0] == COMMENT_TAB[0])
  {
    if (str[0] == COMMENT_RET[0])
      return NULL;
    str++;
  }

  return str;
}

int section_proc(char *str)
{
  int pos;

  pos = strpos(str, "]");
  if (pos < 0)
    return EXIT_SUCCESS;

  configuration_counter++;
  if (configuration_alloc())
    return EXIT_FAILURE;

  if ((configuration[configuration_counter - 1].section = (char *)calloc(pos, sizeof(char))) == NULL)
  {
    return EXIT_FAILURE;
  }
  memcpy(configuration[configuration_counter - 1].section, &(str[1]), pos - 1);
  configuration[configuration_counter - 1].section[pos - 1] = (char)'\0';

  configuration[configuration_counter - 1].cnt = 0;
  configuration[configuration_counter - 1].value = NULL;

  return EXIT_SUCCESS;
}

int value_proc(char *str)
{
  int pos;
  _CONFIG_BUFFER *conf;
  char buf[CONFIG_BUF_SIZE];

  pos = strpos(str, "=");
  if (pos < 0)
    return EXIT_SUCCESS;

  conf = &(configuration[configuration_counter - 1]);
  (conf->cnt)++;
  if (value_alloc(conf) == EXIT_FAILURE)
    return EXIT_FAILURE;

  strcpy(buf, str);
  if (value_lstr_set(conf, buf) == EXIT_FAILURE)
    return EXIT_FAILURE;
  strcpy(buf, str);
  if (value_rstr_set(conf, buf) == EXIT_FAILURE)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int value_lstr_set(_CONFIG_BUFFER *conf, char *buf)
{
  int len;

  if (strtok(buf, "=") == NULL)
  {
    conf->value[conf->cnt - 1].lstr = NULL;
    return EXIT_SUCCESS;
  }
  strcpy(buf, strtok(buf, "="));
  if (strtok(buf, " ") == NULL)
  {
    conf->value[conf->cnt - 1].lstr = NULL;
    return EXIT_SUCCESS;
  }
  strcpy(buf, strtok(buf, " "));
  if (strtok(buf, "\t") == NULL)
  {
    conf->value[conf->cnt - 1].lstr = NULL;
    return EXIT_SUCCESS;
  }
  strcpy(buf, strtok(buf, "\t"));

  len = (int)strlen(buf);
  if (len <= 0)
  {
    conf->value[conf->cnt - 1].lstr = NULL;
    return EXIT_SUCCESS;
  }
  if ((conf->value[conf->cnt - 1].lstr = (char *)calloc(len + 1, sizeof(char))) == NULL)
    return EXIT_FAILURE;
  memcpy(conf->value[conf->cnt - 1].lstr, buf, len);
  conf->value[conf->cnt - 1].lstr[len] = (char)'\0';

  return EXIT_SUCCESS;
}

int value_rstr_set(_CONFIG_BUFFER *conf, char *buf)
{
  int pos, len, change;

  pos = strpos(buf, "=");
  buf = buf + (pos + 1);
  buf = rstrtok(buf);
  if (buf == NULL)
  {
    conf->value[conf->cnt - 1].rstr = NULL;
    return EXIT_SUCCESS;
  }
  if (strtok(buf, " ") == NULL)
  {
    conf->value[conf->cnt - 1].rstr = NULL;
    return EXIT_SUCCESS;
  }
  strcpy(buf, strtok(buf, " "));
  if (strtok(buf, "\t") == NULL)
  {
    conf->value[conf->cnt - 1].rstr = NULL;
    return EXIT_SUCCESS;
  }
  strcpy(buf, strtok(buf, "\t"));
  if (strtok(buf, "\n") == NULL)
  {
    conf->value[conf->cnt - 1].rstr = NULL;
    return EXIT_SUCCESS;
  }
  strcpy(buf, strtok(buf, "\n"));

  change = 1;
  while (change)
  {
    change = 0;
    environment_development(buf, &change);
  }

  len = (int)strlen(buf);
  if (len <= 0)
  {
    conf->value[conf->cnt - 1].rstr = NULL;
    return EXIT_SUCCESS;
  }
  if ((conf->value[conf->cnt - 1].rstr = (char *)calloc(len + 1, sizeof(char))) == NULL)
    return EXIT_FAILURE;
  memcpy(conf->value[conf->cnt - 1].rstr, buf, len);
  conf->value[conf->cnt - 1].rstr[len] = (char)'\0';

  return EXIT_SUCCESS;
}

void environment_development(char *buf, int *change)
{
  int i, pos;
  int st, ed;
  char str[CONFIG_BUF_SIZE], bak[CONFIG_BUF_SIZE];

  st = strpos(buf, "$");
  if (st < 0)
    return;
  if (buf[st + 1] != '{')
    return;
  ed = strpos(buf, "}");
  if (ed < 0)
    return;

  strcpy(bak, &buf[ed + 1]);
  memcpy(str, &buf[st + 2], ed - (st + 2));
  str[ed - (st + 2)] = (char)'\0';
  if (getenv(str))
  {
    strcpy(&buf[st], getenv(str));
    strcpy(&buf[st + (int)strlen(getenv(str))], bak);
    *change = 1;
  }
  else
  {
    pos = -1;
    for (i = 0; i < configuration_counter; i++)
    {
      if (strcmp(configuration[i].section, RESERVE) == 0)
      {
        pos = i;
        break;
      }
    }
    if (pos < 0)
      return;
    for (i = 0; i < configuration[pos].cnt; i++)
    {
      if (strcmp(configuration[pos].value[i].lstr, str) == 0)
      {
        strcpy(&buf[st], configuration[pos].value[i].rstr);
        strcpy(&buf[st + (int)strlen(configuration[pos].value[i].rstr)], bak);
        *change = 1;
        break;
      }
    }
  }
}

int strpos(char *str, const char *c)
{
  int pos;

  pos = 0;
  while (str[pos] != '\n' && str[pos] != (char)'\0')
  {
    if (str[pos] == c[0])
      return pos;
    pos++;
  }

  return -1;
}

int configuration_alloc(void)
{
  if (configuration)
  {
    if (!(configuration = (_CONFIG_BUFFER *)realloc(configuration, configuration_counter * sizeof(_CONFIG_BUFFER))))
      return EXIT_FAILURE;
  }
  else
  {
    if (!(configuration = (_CONFIG_BUFFER *)calloc(configuration_counter, sizeof(_CONFIG_BUFFER))))
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int value_alloc(_CONFIG_BUFFER *conf)
{
  if (conf->value)
  {
    if ((conf->value = (_CONFIG_VALUE *)realloc(conf->value, conf->cnt * sizeof(_CONFIG_VALUE))) == NULL)
      return EXIT_FAILURE;
  }
  else
  {
    if ((conf->value = (_CONFIG_VALUE *)calloc(conf->cnt, sizeof(_CONFIG_VALUE))) == NULL)
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

char *ConfigGet(const char *section, const char *target)
{
  int i, pos_i, pos_j;

  pos_i = -1;
  for (i = 0; i < configuration_counter; i++)
  {
    if (strcmp(section, configuration[i].section) == 0)
    {
      pos_i = i;
      break;
    }
  }
  if (pos_i < 0)
    return NULL;

  pos_j = -1;
  for (i = 0; i < configuration[pos_i].cnt; i++)
  {
    if (configuration[pos_i].value[i].lstr == NULL)
      continue;
    if (strcmp(target, configuration[pos_i].value[i].lstr) == 0)
    {
      pos_j = i;
      break;
    }
  }
  if (pos_j < 0)
    return NULL;

  return configuration[pos_i].value[pos_j].rstr;
}
