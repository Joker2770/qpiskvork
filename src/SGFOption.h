/*************************************************************************
    > File Name: SGFOption.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
    > Created Time: Sat 06 Jul 2024 09:50:28 AM CST
 ************************************************************************/

/**
    qpiskvork is another gomoku or renju manager adapting to Windows and Linux systems.
    Copyright (C) 2024  Jintao Yang <yjt950840@outlook.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef __SGFOPTION_H__
#define __SGFOPTION_H__

#include <libsgfcplusplus/SgfcPlusPlusFactory.h>
#include <libsgfcplusplus/ISgfcArguments.h>
#include <libsgfcplusplus/ISgfcDocument.h>
#include <libsgfcplusplus/ISgfcDocumentReader.h>
#include <libsgfcplusplus/ISgfcDocumentWriter.h>
#include <libsgfcplusplus/ISgfcDocumentReadResult.h>
#include <libsgfcplusplus/ISgfcDocumentWriteResult.h>
#include <libsgfcplusplus/ISgfcGame.h>
#include <libsgfcplusplus/ISgfcMovePropertyValue.h>
#include <libsgfcplusplus/ISgfcNumberPropertyValue.h>
#include <libsgfcplusplus/ISgfcPropertyFactory.h>
#include <libsgfcplusplus/ISgfcPropertyValueFactory.h>
#include <libsgfcplusplus/ISgfcTreeBuilder.h>

#include <iostream>

using namespace LibSgfcPlusPlus;

class SGFOption
{
public:
    SGFOption();
    ~SGFOption();
    std::string idx_2_s(const std::pair<int, int> &idx);
    std::pair<int, int> s_2_idx(const std::string &propertyValue);
    void PrintMessages(const std::vector<std::shared_ptr<ISgfcMessage>> &messages);
    void PrintDocumentContent(std::shared_ptr<ISgfcDocument> document);
    std::shared_ptr<ISgfcDocument> ReadDocument(const std::string &inputFilePath, bool printOutput);
    int WriteDocument(std::shared_ptr<ISgfcDocument> document, const std::string &outputFilePath);
    void record_2_sgf(const std::string &outputFilePath, std::vector<std::pair<int, int>> &vRecord, unsigned int bSize);
    int loadSgf(const std::string &inputFilePath, std::vector<std::pair<int, int>> &vRecord, unsigned int *bSize);
};

#endif
