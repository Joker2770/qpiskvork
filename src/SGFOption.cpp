/*************************************************************************
    > File Name: SGFOption.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
    > Created Time: Sat 06 Jul 2024 09:50:47 AM CST
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

#include "SGFOption.h"
#include "board.h"

SGFOption::SGFOption()
{
}

SGFOption::~SGFOption()
{
}

std::string SGFOption::idx_2_s(const std::pair<int, int> &idx)
{
  std::string sTmp;
  sTmp.clear();
  if (idx.first >= 0 &&
      idx.second >= 0 &&
      idx.first <= MAXBSIZE &&
      idx.second <= MAXBSIZE)
  {
    char cX = 0;
    char cY = 0;
    cX = idx.first + 'a';
    cY = idx.second + 'a';
    sTmp.insert(0, 1, cX);
    sTmp.insert(1, 1, cY);
  }
  return sTmp;
}

/// @brief Prints the raw message text of all ISgfcMessage objects in the
/// supplied collection to standard output.
///
/// This is a helper function.
void SGFOption::PrintMessages(const std::vector<std::shared_ptr<ISgfcMessage>> &messages)
{
  int numberOfMessages = 0;
  for (const auto &message : messages)
  {
    numberOfMessages++;
    std::cout << "  " << message->GetFormattedMessageText() << std::endl;
  }
  if (numberOfMessages == 0)
  {
    std::cout << "  No messages." << std::endl;
  }
  std::cout << std::endl;
}

/// @brief Prints the content of @a document to standard output.
///
/// This is a helper function.
void SGFOption::PrintDocumentContent(std::shared_ptr<ISgfcDocument> document)
{
  std::cout << "Printing document content..." << std::endl;

  if (document->IsEmpty())
    std::cout << "  Document is empty" << std::endl;
  else
    document->DebugPrintToConsole();

  std::cout << std::endl;
}

/// @brief Writes the content of @a document to the SGF file @a outputFilePath.
///
/// This demonstrates the usage of ISgfcDocumentWriter.
int SGFOption::WriteDocument(std::shared_ptr<ISgfcDocument> document, const std::string &outputFilePath)
{
  std::shared_ptr<ISgfcDocumentWriter> documentWriter = SgfcPlusPlusFactory::CreateDocumentWriter();

  // Uncomment the following to see an invalid command line
  //  documentWriter->GetArguments()->AddArgument(SgfcArgumentType::DeletePropertyType, SgfcPropertyType::BO);

  std::shared_ptr<ISgfcDocumentWriteResult> result = documentWriter->WriteSgfFile(document, outputFilePath);

  std::cout << "WriteSgfFile sgfcExitCode = " << static_cast<int>(result->GetExitCode()) << std::endl;
  std::cout << std::endl;

  std::cout << "Printing write results..." << std::endl;
  PrintMessages(result->GetParseResult());

  if (result->GetExitCode() != SgfcExitCode::FatalError)
  {
    std::cout << "Printing encoded SGF content..." << std::endl;
    documentWriter->DebugPrintToConsole(document);
  }

  return 0;
}

void SGFOption::record_2_sgf(const std::string &outputFilePath, std::vector<std::pair<int, int>> &vRecord, unsigned int bSize)
{
  Board bTmp;
  std::shared_ptr<ISgfcDocument> document = SgfcPlusPlusFactory::CreateDocument();
  std::shared_ptr<ISgfcGame> game = document->GetGame();
  std::shared_ptr<ISgfcNode> rootNode = game->GetRootNode();

  std::shared_ptr<ISgfcNode> node[vRecord.size()];

  auto builder = game->GetTreeBuilder();

  auto propertyValueFactory = SgfcPlusPlusFactory::CreatePropertyValueFactory();
  auto propertyValueGM = propertyValueFactory->CreateNumberPropertyValue(4);
  auto propertyValueSZ = propertyValueFactory->CreateNumberPropertyValue(bSize);

  auto propertyFactory = SgfcPlusPlusFactory::CreatePropertyFactory();
  std::shared_ptr<ISgfcProperty> propertyGM = propertyFactory->CreateProperty(SgfcPropertyType::GM, propertyValueGM);
  std::shared_ptr<ISgfcProperty> propertySZ = propertyFactory->CreateProperty(SgfcPropertyType::SZ, propertyValueSZ);

  rootNode->SetProperties(std::vector<std::shared_ptr<ISgfcProperty>>{propertyGM, propertySZ});

  for (size_t i = 0; i < vRecord.size(); i++)
  {
    node[i] = SgfcPlusPlusFactory::CreateNode();
    if (0 == i)
    {
      builder->AppendChild(rootNode, node[0]);
    }
    else
    {
      builder->AppendChild(node[i - 1], node[i]);
    }

    auto propertyValueMove = propertyValueFactory->CreateMovePropertyValue(idx_2_s(bTmp.coord2idx(vRecord.at(i).first)));
    std::shared_ptr<ISgfcProperty> property;
    if (STONECOLOR::BLACK == vRecord.at(i).second)
    {
      property = propertyFactory->CreateProperty(SgfcPropertyType::B, propertyValueMove);
      node[i]->SetProperties(std::vector<std::shared_ptr<ISgfcProperty>>{property});
    }
    else if (STONECOLOR::WHITE == vRecord.at(i).second)
    {
      property = propertyFactory->CreateProperty(SgfcPropertyType::W, propertyValueMove);
      node[i]->SetProperties(std::vector<std::shared_ptr<ISgfcProperty>>{property});
    }
    else
    {
      // do nothing
    }
  }

  PrintDocumentContent(document);

  WriteDocument(document, outputFilePath);
}
