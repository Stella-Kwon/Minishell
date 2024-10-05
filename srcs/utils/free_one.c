/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_one.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 20:42:39 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/03 23:35:21 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

#include <stdlib.h>

void free_one(void **ptr)
{
    if (ptr && *ptr)
    {
        free(*ptr); 
        *ptr = NULL;
    }
}
