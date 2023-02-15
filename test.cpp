/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-amma <mel-amma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 14:50:40 by mel-amma          #+#    #+#             */
/*   Updated: 2023/02/14 15:07:33 by mel-amma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ChunckContentHandler.hpp"

int main()
{
    ChunkContentHandler handler;
    std::vector<std::string> s(2, "");
    s[0] = "9bf\r\nLe massacre de Nemmersdorf est un massacre de civils allemands survenu entre le 21 et le 23 octobre 1944 dans le village, alors allemand, de Nemmersdorf en Prusse orientale (aujourdahui Maïakovskoïe, en Russie). Au cours de ce massacre, laArmée rouge tue entre 20 et 30 personnes après la prise de cette localité. Les victimes de Nemmersdorf ne sont pas toutes tuées au même moment ou au même endroit dans le village : le terme de « massacre » désigne en réalité une succession daévénements distincts.\
Capturé par les Soviétiques le 21 octobre 1944, le village de Nemmersdorf est repris dans la nuit du 23 octobre par les Allemands, qui découvrent des exactions commises par laArmée rouge sur les civils. Rapidement, les autorités militaires allemandes enquêtent sur ces événements, et le ministère de laÉducation du peuple et de la Propagande du Reich saen empare pour faire sa propagande. Le massacre de Nemmersdorf sert notamment à présenter les soldats soviétiques comme cruels et barbares, dans le but de galvaniser la résistance allemande. Cet objectif naest toutefois pas atteint puisque caest la peur des Allemands qui saen trouve renforcée, ce qui engendre plus tard dans la guerre des réactions de panique face à laavancée des troupes soviétiques, comme lors des suicides de Demmin.\
Après la fin de la guerre, lainstrumentalisation des événements de Nemmersdorf se poursuit. En effet, la République fédérale daAllemagne érige ces crimes de guerre en symbole de ce que la population daAllemagne de laEst est alors sous occupation soviétique. Les descriptions héritées de la propagande nazie sont encore amplifiées dans un contexte de guerre froide. De laautre côté du rideau de fer, la réalité du massacre de Nemmersdorf est nié";
    s[1] = "e, et ces événements sont surtout présentés comme une opération de propagande nazie.\
Avec la fin de la guerre froide et l'étude plus neutre des crimes de guerres subis par le peuple allemand dans les derniers mois de la Seconde Guerre mondiale, l'historiographie du massacre de Nemmersdorf est devenue plus apaisée. Les travaux de Bernhard Fisch au tournant des années 2000, particulièrement, permirent une réévaluation de ces événements. En Allemagne, le massacre de Nemmersdorf est encore emblématique des violences infligées par l'Armée rouge, au même titre que les bombardements de villes comme Dresde ou Hambourg sont emblématiques des violences infligées par les Alliés occidentaux.\r\n0\r\n";
    for (int  i = 0 ; i < 2; i++)
    {
         std::vector<const char *>  chunks;
        if(!handler.getHttpChunkContent(s[i].c_str(),s[i].length(),chunks))
        {
            std::cout << "chunkde failed\n";
        }
        // for (int i = 0; i < chunks.size(); i +=2)
        // {
        //     std::cout << " chunk number = " << i / 2 << std::endl;
        //     const char *a =  chunks[i];
        //     std::string currChunk;
        //     while (a !=  chunks[i + 1])
        //     {
        //         currChunk += *a;
        //         a++;
        //     }
        //     std::cout << currChunk << std::endl;
        // }
    }
    return (0);
}