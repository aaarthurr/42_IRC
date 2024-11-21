/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:38:30 by arthur            #+#    #+#             */
/*   Updated: 2024/11/20 15:19:06 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma	once

#include <string>
#include <vector>
#include <map>
#include <set>
#include "irc.hpp"

class Channel {
private:
    std::string name;                     // Nom du canal (ex: #general)
    std::string topic;                    // Sujet du canal
    std::set<std::string> users;          // Liste des utilisateurs dans le canal
    std::map<char, bool> modes; // Modes du canal (par ex: +p, +m, +t)

public:
    // Constructeur
	Channel();
    Channel(const std::string& channelName);
	~Channel();

    // Getter pour le nom du canal
    const std::string& getName() const { return name; }

    // Gestion des utilisateurs
    void addUser(const std::string& user);
    void removeUser(const std::string& user);
    bool hasUser(const std::string& user) const;

    // Gestion des modes
    void setMode(char mode, bool value);
    bool getMode(char mode) const;

    // Gestion du sujet
    void setTopic(const std::string& newTopic);
    const std::string& getTopic() const;

    // Méthode pour diffuser un message à tous les utilisateurs
    void broadcastMessage(const std::string& sender, const std::string& message);
};
