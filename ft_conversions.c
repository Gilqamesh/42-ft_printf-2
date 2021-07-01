/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conversions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 14:31:19 by edavid            #+#    #+#             */
/*   Updated: 2021/07/01 20:45:59 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft/libft.h"
#include "ft_conversions.h"

static void	positive_width(int *printed_bytes, int *flags, unsigned char c)
{
	*printed_bytes = flags[2] + 1;
	if (flags[0])
	{
		write(1, &c, 1);
		while (flags[2]--)
			ft_putchar_fd(' ', 1);	
	}
	else
	{
		while (flags[2]--)
			ft_putchar_fd(' ', 1);	
		write(1, &c, 1);
	}
}

int	print_conversion_c(unsigned char c, int *flags)
{
	int	printed_bytes;

	if (--flags[2] > 0)
		positive_width(&printed_bytes, flags, c);
	else
	{
		printed_bytes = 1;
		write(1, &c, 1);
	}
	return (printed_bytes);
}

int	print_conversion_uint(unsigned int n, int *flags)
{
	char	*converted_str;
	int		conv_str_len;
	int		precision;
	int		printed_bytes;

	if (flags[3] == -3)	// no precision
		precision = -1;
	else if (flags[3] == -1) 	// read from *
		precision = flags[4];
	else						// has precision
		precision = flags[3];
	converted_str = ft_utoa(n);
	if (flags[3] == -2 && !n)	// if 0 precision and n is 0
		shift_str(&converted_str);
	conv_str_len = ft_strlen(converted_str);

	if (precision > conv_str_len) // pad precision - conv_str_len 0s
	{
		if (flags[2] > precision) // space padded by flags[2] - precision
		{
			printed_bytes = flags[2];
			if (flags[0]) // left justified
			{
				while (precision-- - conv_str_len)
				{
					ft_putchar_fd('0', 1);
					flags[2]--;
				}
				ft_putstr_fd(converted_str, 1);
				while (flags[2]-- - conv_str_len)
					ft_putchar_fd(' ', 1);
			}
			else // right justified
			{
				while (flags[2]-- - precision)
					ft_putchar_fd(' ', 1);
				while (precision-- - conv_str_len)
					ft_putchar_fd('0', 1);
				ft_putstr_fd(converted_str, 1);
			}
		}
		else // not space padded, left justified by default
		{
			printed_bytes = precision;
			while (precision-- - conv_str_len)
				ft_putchar_fd('0', 1);
			ft_putstr_fd(converted_str, 1);
		}
	}
	else // precision less than or equal to str_len
	{
		if (flags[2] > conv_str_len) // padded
		{
			printed_bytes = flags[2];
			if (flags[0])
			{
				if (flags[1] && precision == -1) // 0 padded instead of space
				{
					while (flags[2]-- - conv_str_len)
						ft_putchar_fd('0', 1);
					ft_putstr_fd(converted_str, 1);
				}
				else
				{
					ft_putstr_fd(converted_str, 1);
					while (flags[2]-- - conv_str_len)
						ft_putchar_fd(' ', 1);
				}
			}
			else // right justified
			{
				if (flags[1] && precision == -1)
				{
					while (flags[2]-- - conv_str_len)
						ft_putchar_fd('0', 1);
				}
				else
					while (flags[2]-- - conv_str_len)
						ft_putchar_fd(' ', 1);
				ft_putstr_fd(converted_str, 1);
			}
		}
		else // no padding
		{
			ft_putstr_fd(converted_str, 1);
			printed_bytes = conv_str_len;
		}
	}
	return (printed_bytes);
}
