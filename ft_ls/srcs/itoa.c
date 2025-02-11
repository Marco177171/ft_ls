#include "ft_ls.h"

char	*ft_array(char *result, unsigned int number, long int len)
{
	while (number > 0)
	{
		result[len--] = 48 + (number % 10);
		number = number / 10;
	}
	return (result);
}

int	ft_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		len++;
		n = n / 10;
	}
	return (len);
}

char	*ft_itoa(int n) {
	char			*result;
	unsigned int	len;
	unsigned int	number;
	int				sign;

	sign = 1;
	len = ft_len(n);
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!(result))
		return (NULL);
	result[len--] = '\0';
	if (n == 0)
		result[0] = '0';
	if (n < 0)
	{
		sign *= -1;
		number = n * sign;
		result[0] = '-';
	}
	else
		number = n;
	result = ft_array(result, number, len);
	return (result);
}