/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <vdescham@student.42.fr           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 12:36:53 by vdescham          #+#    #+#             */
/*   Updated: 2022/01/05 01:41:00 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

# include "headers.hpp"

std::string		setLink(struct stat &buf, std::string dname);
std::string		setSize(struct stat &buf);
std::string		setCreationTime(struct stat &buf);
std::string		setModificationTime(struct stat &buf);
std::string 	createDirList(std::string path, std::string dname);
std::string		createFirstPart(std::string &path);

std::string		createAutoindexPage(std::string path);

#endif
