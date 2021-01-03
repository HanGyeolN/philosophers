#include "philo_three.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(long nb)
{
	long long int n;
	long long int div;
	long long int mod;

	n = (long long int)nb;
	if (n < 0)
	{
		n = -n;
		ft_putchar('-');
	}
	div = n / 10;
	mod = n % 10;
	if (n >= 10)
	{
		ft_putnbr(div);
	}
	ft_putchar(mod + '0');
}

int		ft_strlen(char *str)
{
	int		i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

int		ft_atoi(const char *s)
{
	int		i;
	int		ret;
	int		sign;

	i = 0;
	ret = 0;
	sign = 1;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -sign;
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		ret = ret * 10 + (s[i] - '0') * sign;
		i++;
	}
	return (ret);
}