#include "minishell.h"

// static void    ft_putstr(char *s)
// {
//     int i;

//     i = 0;
//     while (s[i])
//     {
//         write(1,&s[i],1);
//         i++;
//     }
// }
int count_len_of_str(char *s[])
{
    int i;

    i = 0;
    while (s[i])
    {
        i++;
    }
    return i;
}
int is_n(char *str)
{
    int i;

    i = 0;
    if(str == NULL)
        return 0;
    if (str[i] == '-')
        i++;
    while (str[i] && str[i] != 32)
    {
        if (str[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}
int if_more_n(char *s)
{
    int i;

    i = 0;
    if(s[i] != '-')
        return 1;
    while(s[i])
    {
        if(s[i] != '-' && s[i] != 'n')
            return 1;
        i++;
    }
    return 0;
}
void    handle_echo(t_cmd_line *head)
{
    int f;
    int len;
    int i;

    f = 0;
    i = 1;
    if (head->str[1] == NULL)
    {
        ft_putstr("\n");
        return ;
    }
    while (is_n(head->str[i]))
    {
        f = 1;
        if(if_more_n(head->str[i]))
        {
            // f = 0;
            break;
        }
        i++;
    }
    if(i > 2)
    {
        printf("pd");
        f = 1;
    }
    len = count_len_of_str(head->str);
    while (i < len )
    {
       ft_putstr(head->str[i]);
       if (head->str[i + 1])
         ft_putstr(" ");
       i++;
    }
    if (f == 0)
        ft_putstr("\n");
}