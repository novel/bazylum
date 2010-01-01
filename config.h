#if !defined(__CONFIG_H)
#define __CONFIG_H

#define CONFIG_PATH	"~/.bazylum"

short int does_config_exist();
void create_config_if_not_exists();

#endif /* __CONFIG_H */
