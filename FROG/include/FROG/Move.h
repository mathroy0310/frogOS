/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Move.h                                       ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:16:43 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/05 01:16:44 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace FROG {

template <class T> struct RemoveReference {
  typedef T type;
};

template <class T> struct RemoveReference<T &> {
  typedef T type;
};

template <class T> struct RemoveReference<T &&> {
  typedef T type;
};

template <class T> typename RemoveReference<T>::type &&Move(T &&Arg) {
  return (typename RemoveReference<T>::type &&)Arg;
}

} // namespace FROG