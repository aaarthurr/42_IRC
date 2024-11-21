/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:38:32 by arthur            #+#    #+#             */
/*   Updated: 2024/11/20 15:22:47 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// Constructeur principal
Channel::Channel(const std::string& channelName) : name(channelName), topic("")
{
}

// Constructeur par défaut
Channel::Channel() : name("#general"), topic("")
{
}

// Destructeur
Channel::~Channel() {
}

// Ajouter un utilisateur
void Channel::addUser(const std::string& user) {
    users.insert(user);
}

// Retirer un utilisateur
void Channel::removeUser(const std::string& user) {
    users.erase(user);
}

// Vérifier si un utilisateur est présent
bool Channel::hasUser(const std::string& user) const {
    return users.find(user) != users.end();
}

// Définir un mode pour le canal
void Channel::setMode(char mode, bool value) {
    modes[mode] = value;
}

// Obtenir la valeur d'un mode
bool Channel::getMode(char mode) const {
    std::map<char, bool>::const_iterator it = modes.find(mode);
    return it != modes.end() && it->second;
}

// Définir le sujet du canal
void Channel::setTopic(const std::string& newTopic) {
    topic = newTopic;
}

// Obtenir le sujet du canal
const std::string& Channel::getTopic() const {
    return topic;
}

// Diffuser un message à tous les utilisateurs
void Channel::broadcastMessage(const std::string& sender, const std::string& message) {
    std::set<std::string>::const_iterator it;
    for (it = users.begin(); it != users.end(); ++it) {
        // Simuler l'envoi de message à l'utilisateur
        std::cout << "Message to " << *it << ": [" << name << "] "
                  << sender << ": " << message << std::endl;
    }
}
