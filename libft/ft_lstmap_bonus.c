/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:39:19 by hlee-sun          #+#    #+#             */
/*   Updated: 2023/11/22 20:28:44 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*map;
	t_list	*tmp;
	void	*modi;

	if (!lst || !f)
		return (NULL);
	map = NULL;
	while (lst != NULL)
	{
		modi = f(lst->content);
		tmp = ft_lstnew(modi);
		if (!tmp)
		{
			if (del != NULL)
				del(modi);
			ft_lstclear(&map, del);
			return (NULL);
		}
		ft_lstadd_back(&map, tmp);
		lst = lst->next;
	}
	return (map);
}
