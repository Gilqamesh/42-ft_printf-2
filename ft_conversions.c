/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conversions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 14:31:19 by edavid            #+#    #+#             */
/*   Updated: 2021/07/01 15:01:31 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_conversions.h"
#include "libft/libft.h"

int	print_conversion_c(unsigned char c, int *flags)
{
	int	printed_bytes;

	if (--flags[2] > 0) // pad with spaces
	{
		printed_bytes = flags[2] + 1;
		if (flags[0]) // left justified
		{
			write(1, &c, 1);
			while (flags[2]--)
				ft_putchar_fd(' ', 1);	
		}
		else // right justified
		{
			while (flags[2]--)
				ft_putchar_fd(' ', 1);	
			write(1, &c, 1);
		}
	}
	else
	{
		printed_bytes = 1;
		write(1, &c, 1);
	}
	return (printed_bytes);
}

int	print_conversion_s(char *str, int *flags)
{
	int	str_len;
	int printed_bytes;
	int	precision;
	int	i;
	int is_null;

	is_null = 0;
	if (!str)
	{
		is_null = 1;
		str = "(null)";
	}
	if (flags[3] == -2)			// 0 precision
		precision = 0;
	else if (flags[3] == -3) 	// no precision
		precision = -1;
	else if (flags[3] == -1) 	// read from *
		precision = flags[4];
	else						// has precision
		precision = flags[3];
	printed_bytes = 0;
	str_len = ft_strlen(str);
	if (precision > str_len && flags[3] != -1)
		precision = str_len;
	// printf("precision: %d\n", precision);
	if (precision == -1) // no truncation
	{
		if (flags[2] > str_len) // padding
		{
			printed_bytes = flags[2];
			if (flags[0]) // left justified
			{
				ft_putstr_fd(str, 1);
				while (flags[2]-- - str_len)
					ft_putchar_fd(' ', 1);
			}
			else // right justified
			{
				while (flags[2]-- - str_len)
					ft_putchar_fd(' ', 1);
				ft_putstr_fd(str, 1);
			}
		}
		else // no padding
		{
			printed_bytes = str_len;	
			ft_putstr_fd(str, 1);
		}
	}
	else if (flags[2] > precision) // need to pad
	{
		if (precision < str_len) // truncation
		{
			printed_bytes = flags[2];
			if (flags[0]) // left justified
			{
				i = -1;
				while (++i < precision)
					write(1, str++, 1);
				while (flags[2]-- - precision)
					ft_putchar_fd(' ', 1);
			}
			else // right justified
			{
				while (flags[2]-- - precision)
					ft_putchar_fd(' ', 1);
				while (precision--)
					write(1, str++, 1);
			}
		}
		else // no truncation
		{
			printed_bytes = flags[2];
			if (flags[0]) // left justified
			{
				ft_putstr_fd(str, 1);
				while (flags[2]-- - str_len)
					ft_putchar_fd(' ', 1);
			}
			else // right justified
			{
				while (flags[2]-- - str_len)
					ft_putchar_fd(' ', 1);
				ft_putstr_fd(str, 1);
			}
		}
	}
	else if (flags[2] > str_len) // need to pad with spaces
	{
		printed_bytes = flags[2];
		if (flags[0]) // left justified
		{
			ft_putstr_fd(str, 1);
			while (flags[2]-- - str_len)
				ft_putchar_fd(' ', 1);
		}
		else
		{
			while (flags[2]-- - str_len)
				ft_putchar_fd(' ', 1);
			ft_putstr_fd(str, 1);
		}
	}
	else 
	{
		if (precision < str_len) // truncation
		{
			printed_bytes = precision;
			while (precision--)
				write(1, str++, 1);
		}
		else
		{
			printed_bytes = str_len;
			ft_putstr_fd(str, 1);
		}
	}
	return (printed_bytes);
}

int	print_conversion_p(void *arg_pointer, int *flags)
{
	int addr_len;
	int	printed_spaces;
	int	null_precision_pointer;

	null_precision_pointer = 0;
	if (!arg_pointer && flags[3] == -2)
		null_precision_pointer = 1;
	printed_spaces = 0;
	addr_len = digits_in_hexa((unsigned long)arg_pointer);
	if (flags[0])
	{
		write(1, "0x", 2);
		if (!null_precision_pointer)
			print_ultoh((unsigned long)arg_pointer, 's');
		if (flags[2] > addr_len + 2 - null_precision_pointer)
			while (flags[2]-- > addr_len + 2 - null_precision_pointer)
			{
				ft_putchar_fd(' ', 1);
				printed_spaces++;
			}
	}
	else
	{
		if (flags[2] > addr_len + 2 - null_precision_pointer)
			while (flags[2]-- > addr_len + 2 - null_precision_pointer)
			{
				ft_putchar_fd(' ', 1);
				printed_spaces++;
			}
		write(1, "0x", 2);
		if (!null_precision_pointer)
			print_ultoh((unsigned long)arg_pointer, 's');
	}
	return (addr_len + printed_spaces + 2);
}

int	print_conversion_int(int n, int *flags)
{
	char	*converted_str;
	int		conv_str_len;
	int		precision;
	int		printed_bytes;
	int		is_negative;

	if (flags[3] == -3)	// no precision
		precision = -1;
	else if (flags[3] == -1) 	// read from *
		precision = flags[4];
	else						// has precision
		precision = flags[3];
	is_negative = 0;
	if (n < 0)
		is_negative = 1;
	converted_str = ft_itoa(n);
	if (is_negative)
		shift_str(&converted_str);
	if (flags[3] == -2 && !n)	// if 0 precision and n is 0
		shift_str(&converted_str);
	conv_str_len = ft_strlen(converted_str);

	// printf("In print_conversion_int, n: %d conv_str: %s\n", n, converted_str);
	// printf("Converted string len: %d\n", conv_str_len);

	if (precision > conv_str_len) // pad precision - conv_str_len 0s
	{
		if (flags[2] > precision) // space padded by flags[2] - precision
		{
			printed_bytes = flags[2];
			if (flags[0]) // left justified
			{
				if (is_negative)
					ft_putchar_fd('-', 1);
				while (precision - conv_str_len++)
					ft_putchar_fd('0', 1);
				ft_putstr_fd(converted_str, 1);
				while (flags[2]-- - precision - is_negative)
					ft_putchar_fd(' ', 1);
			}
			else // right justified
			{
				while (flags[2]-- - precision - is_negative)
					ft_putchar_fd(' ', 1);
				if (is_negative)
					ft_putchar_fd('-', 1);
				while (precision-- - conv_str_len)
					ft_putchar_fd('0', 1);
				ft_putstr_fd(converted_str, 1);
			}
		}
		else // not space padded, left justified by default
		{
			printed_bytes = precision + is_negative;
			if (is_negative)
				ft_putchar_fd('-', 1);
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
			if (flags[0]) // left justified
			{
				if (flags[1] && precision == -1) // 0 padded instead of space
				{
					if (is_negative)
						ft_putchar_fd('-', 1);
					while (flags[2]-- - conv_str_len - is_negative)
						ft_putchar_fd('0', 1);
					ft_putstr_fd(converted_str, 1);
				}
				else
				{
					if (is_negative)
						ft_putchar_fd('-', 1);
					ft_putstr_fd(converted_str, 1);
					while (flags[2]-- - conv_str_len - is_negative)
						ft_putchar_fd(' ', 1);
				}
			}
			else // right justified
			{
				if (flags[1] && precision == -1)
				{
					if (is_negative)
						ft_putchar_fd('-', 1);
					while (flags[2]-- - conv_str_len - is_negative)
						ft_putchar_fd('0', 1);
				}
				else
				{
					while (flags[2]-- - conv_str_len - is_negative)
						ft_putchar_fd(' ', 1);
					if (is_negative)
						ft_putchar_fd('-', 1);
				}
				ft_putstr_fd(converted_str, 1);
			}
		}
		else // no padding
		{
			printed_bytes = conv_str_len + is_negative;
			if (is_negative)
				ft_putchar_fd('-', 1);
			ft_putstr_fd(converted_str, 1);
		}
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
