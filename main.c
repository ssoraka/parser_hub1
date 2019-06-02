#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>
#define size_t unsigned long

size_t	ft_strlen(const char *str)
{
	size_t i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putstr_fd(char const *s, int fd)
{
	write(fd, s, ft_strlen(s));
}


char	*ft_strcpy(char *dest, const char *src)
{
	unsigned int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*str;

	i = 0;
	str = s;
	while (i < n)
		str[i++] = '\0';
}

void	*ft_memalloc(size_t size)
{
	void	*str;

	str = (void*)malloc(size);
	if (str == 0)
		return (NULL);
	ft_bzero(str, size);
	return (str);
}

char	*ft_strnew(size_t size)
{
	char	*str;

	str = NULL;
	if (size + 1 == 0)
		return (NULL);
	if (!(str = (char *)ft_memalloc(sizeof(char) * (size + 1))))
		return (NULL);
	return (str);
}

char	*ft_strcat(char *dest, const char *src)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
		i++;
	while (src[j] != '\0')
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	max;

	max = -1;
	if (s1 == 0 || s2 == 0)
		return (NULL);
	if (max - ft_strlen(s1) < ft_strlen(s2))
		return (NULL);
	if (!(str = ft_strnew(ft_strlen(s1) + ft_strlen(s2))))
		return (NULL);
	ft_strcpy(str, s1);
	ft_strcat(str, s2);
	return (str);
}

char	*ft_strjoin_free(char *str1, char *str2, int del1, int del2)
{
	char	*str;

	if (str1 == NULL)
	{
		del1 = 1;
		str1 = ft_strnew(0);
	}
	if (str2 == NULL)
	{
		del2 = 1;
		str2 = ft_strnew(0);
	}
	str = ft_strjoin(str1, str2);
	if (del1 > 0)
		free(str1);
	if (del2 > 0)
		free(str2);
	return (str);
}

void	ft_write_all_file(int fd, char **str)
{
	char		buf[1000 + 1];
	int			readf;

	if (read(fd, NULL, 0) < 0)
		return ;
	while ((readf = read(fd, buf, 1000)) > 0)
	{
		buf[readf] = '\0';
		*str = ft_strjoin_free(*str, buf, 1, 0);
	}
}

void	ft_write_logs(char *str, char *name)
{
	int file;

	file = open(name, O_RDWR | O_CREAT | O_APPEND | O_TRUNC);
	if (file < 0)
		file = 1;
	ft_putstr_fd(str, file);
	close(file);
}


char	*ft_strndup(const char *src, size_t n)
{
	char	*str;
	size_t	i;

	i = 0;
	if (ft_strlen(src) < n)
		n = ft_strlen(src);
	str = (char*)ft_memalloc(sizeof(char) * (n + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (i < n)
	{
		str[i] = src[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strstr(const char *str, const char *to_find)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (str[i] == '\0' && to_find[i] == '\0')
		return ((char *)str);
	while (str[i] != '\0')
	{
		j = 0;
		while (str[i + j] == to_find[j] && to_find[j] != '\0')
			j++;
		if (to_find[j] == '\0')
			return ((char *)str + i);
		i++;
	}
	return (NULL);
}

char	*ft_find_pattern(char *str)
{
	char	*answer;
	char	*temp;
	int		fail;

	fail = 0;
	answer = ft_strstr(str, "icmppingloss");
	if (answer)
		answer = ft_strstr(answer, "value");
	if (answer)
		answer = answer + 8;
	temp = answer;
	while (temp && *temp != ',' && *temp != '}')
		temp++;
	if (answer)
		answer = ft_strndup(answer, temp - answer);
	free(str);
	return (answer);
}


int		main(int argc, char **argv)
{
	char	*str;
	int		fd;

	str = NULL;
	if (argc < 2)
	{//не послан аргумент в виде названия файла
		ft_write_logs("не послан аргумент в виде названия файла", "logs.txt");
		return (0);
	}
	fd = open(argv[1], O_RDONLY);// | O_APPEND);
	if (fd < 0)
	{
		ft_write_logs("не получилось открыть файл", "logs.txt");
		return (0);
	}
	ft_write_all_file(fd, &str);
	str = ft_find_pattern(str);
	if (str)
		ft_write_logs(str, "logs.txt");
	else
		ft_write_logs("фрагмент не найден в файле", "logs.txt");
	free(str);
	close(fd);
	return (0);
}
