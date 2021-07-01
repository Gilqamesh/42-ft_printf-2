/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edavid <edavid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 15:43:59 by edavid            #+#    #+#             */
/*   Updated: 2021/07/01 15:49:19 by edavid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_printf.h"
#include "libft/libft.h"
#include "ft_conversions.h"

char	*malloc_conv_spec(char *format, int *format_index) // shave 10 lines
{
	int		index;
	int		format_len;
	char	*conversion_specifier; 

	if (!*format)
	{
		conversion_specifier = malloc(1);
		*conversion_specifier = '\0';
		return (conversion_specifier);
	}
	index = 0;
	while (format[index] == '-' || format[index] == '0')
		index++;
	while (ft_isdigit(format[index]))
		index++;
	if (format[index] == '*')
	{
		index++;
	}
	if (format[index] == '.')
	{
		index++;
		if (format[index] == '*')
			index++;
		else
			while (ft_isdigit(format[index]))
				index++;
	}
	format_len = index + 1;
	conversion_specifier = malloc(format_len + 1);
	index = -1;
	while (++index < format_len)
		conversion_specifier[index] = format[index];
	conversion_specifier[index] = '\0';
	*format_index += format_len;
	return (conversion_specifier);
}

int		set_flags(char *conv_spec, int *flags, va_list ap) // fairly long
{
	int		conv_spec_index;
	char	c;

	conv_spec_index = 0;
	c = conv_spec[conv_spec_index];
	while (c == '-' || c == '0')
	{
		if (c == '-')
		{
			flags[0] = 1;
			flags[1] = 0;
		}
		else if (c == '0' && !flags[0])
			flags[1] = 1;
		c = conv_spec[++conv_spec_index];
	}
	while (ft_isdigit(c))
	{
		flags[2] = flags[2] * 10 + c - '0';
		c = conv_spec[++conv_spec_index];
	}
	if (c == '*')
	{
		flags[2] = va_arg(ap, int);
		if (flags[2] < 0)
		{
			flags[2] *= -1;
			flags[0] = 1;
			flags[1] = 0;
		}
		c = conv_spec[++conv_spec_index];
	}
	if (c == '.')
	{
		c = conv_spec[++conv_spec_index];
		if (c == '*')
		{
			flags[3] = -1;
			flags[4] = va_arg(ap, int);
			if (flags[4] < 0)
				flags[3] = -3;
			else if (flags[4] == 0)
				flags[3] = -2;
			conv_spec_index++;
		}
		else
		{
			if (!ft_isdigit(c))
				flags[3] = -2;
			else
			{
				while (ft_isdigit(c))
				{
					flags[3] = flags[3] * 10 + c - '0';
					c = conv_spec[++conv_spec_index];
				}
				if (flags[3] == 0)
					flags[3] = -2;
			}
		}
	}
	else
		flags[3] = -3;
	return (conv_spec_index);
}

int	handle_conversion_spec(char *conv_spec, va_list ap) // looks good
{
	int		*flags;
	int		conversion_index;
	int		printed_bytes;

	flags = ft_calloc(5, sizeof(int));
	conversion_index = set_flags(conv_spec, flags, ap);
	printed_bytes = print_conversion(conv_spec[conversion_index], ap, flags);
	free(flags);
	return (printed_bytes);
}

int	is_valid_conv_spec(char *conv_spec) // looks good
{
	if (*conv_spec == '%')
		return (1);
	while (*conv_spec == '-' || *conv_spec == '0')
		conv_spec++;
	while (ft_isdigit(*conv_spec))
		conv_spec++;
	if (*conv_spec == '*')
		conv_spec++;
	if (*conv_spec == '.')
	{
		conv_spec++;
		if (*conv_spec == '*')
			conv_spec++;
		else
			while (ft_isdigit(*conv_spec))
				conv_spec++;
	}
	if (*conv_spec == '\0' || (*conv_spec != 'c' && *conv_spec != 's' &&
	*conv_spec != 'p' && *conv_spec != 'd' && *conv_spec != 'i' &&
	*conv_spec != 'u' && *conv_spec != 'x' && *conv_spec != 'X' &&
	*conv_spec != '%'))
		return (0);
	return (1);
}
