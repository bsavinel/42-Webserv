/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:44:16 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:44:17 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

static void	ft_int_len(int n, int *len)
{
	if (n < 0)
	{
		*len = *len + 1;
		n = n * -1;
	}
	if (n > 9)
	{
		ft_int_len(n / 10, len);
		n = n % 10;
	}
	if (n <= 9)
		*len = *len + 1;
}

static void	ft_fill_array(int n, char *itoa_array, int *pos)
{
	if (n < 0)
	{
		itoa_array[*pos] = '-';
		*pos = *pos + 1;
		n = n * -1;
	}
	if (n > 9)
	{
		ft_fill_array(n / 10, itoa_array, pos);
		n = n % 10;
	}
	if (n <= 9)
	{
		itoa_array[*pos] = n + 48;
		*pos = *pos + 1;
	}
	return ;
}

/* Alloue (avec malloc(3)) et retourne une chaine de
caractères représentant l’integer reçu en argument.
Les nombres négatifs doivent être gérés.
La chaine de caractères représentant l’integer.
NULL si l’allocation échoue.
#1. l’integer à convertir. */
char	*ft_itoa(int n)
{
	int		len;
	char	*itoa_array;
	int		pos;

	pos = 0;
	len = 0;
	ft_int_len(n, &len);
	if (n == -2147483648)
		return (strdup("-2147483648"));
	itoa_array = (char *) malloc((len + 1) * sizeof(char));
	if (!itoa_array)
		return (NULL);
	while (pos < len)
		ft_fill_array(n, itoa_array, &pos);
	itoa_array[len] = '\0';
	return (itoa_array);
}