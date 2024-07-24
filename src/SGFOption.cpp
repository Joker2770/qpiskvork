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

std::pair<int, int> SGFOption::s_2_idx(const std::string &StringPropertyValue)
{
  std::pair<int, int> idx(-1, -1);
  if (!StringPropertyValue.empty() && StringPropertyValue.length() == 2)
  {
    idx.first = StringPropertyValue.at(0) - 'a';
    idx.second = StringPropertyValue.at(1) - 'a';
  }

  return idx;
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

/// @brief Reads the SGF file @a inputFilePath. If @a printOutput is true also
/// prints the document content to standard output.
///
/// This demonstrates the usage of ISgfcDocumentReader.
std::shared_ptr<ISgfcDocument> SGFOption::ReadDocument(const std::string &inputFilePath, bool printOutput)
{
  std::shared_ptr<ISgfcDocumentReader> documentReader = SgfcPlusPlusFactory::CreateDocumentReader();

  // Uncomment the following to see an invalid command line
  //  documentReader->GetArguments()->AddArgument(SgfcArgumentType::DeletePropertyType, SgfcPropertyType::BO);

  std::shared_ptr<ISgfcDocumentReadResult> result = documentReader->ReadSgfFile(inputFilePath);

  if (printOutput)
  {
    std::cout << "ReadSgfFile sgfcExitCode = " << static_cast<int>(result->GetExitCode()) << std::endl;
    std::cout << "IsSgfDataValid = " << result->IsSgfDataValid() << std::endl;
    std::cout << std::endl;

    std::cout << "Printing parse results..." << std::endl;
    PrintMessages(result->GetParseResult());

    if (result->GetExitCode() != SgfcExitCode::FatalError)
      PrintDocumentContent(result->GetDocument());
  }

  return result->GetDocument();
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

void SGFOption::record_2_sgf(const std::string &outputFilePath, std::vector<std::pair<int, int>> &vRecord, unsigned int bSize, const std::string &sDate, const std::string &sBName, const std::string &sWName)
{
  Board bTmp;
  std::shared_ptr<ISgfcDocument> document = SgfcPlusPlusFactory::CreateDocument();
  std::shared_ptr<ISgfcGame> game = document->GetGame();

  std::shared_ptr<ISgfcNode> rootNode = game->GetRootNode();

  // std::shared_ptr<ISgfcGameInfo> gameInfo = game->CreateGameInfo();
  // SgfcSimpleText PW = sWName;
  // SgfcSimpleText PB = sBName;
  // SgfcSimpleText DT = sDate.empty()?"":sDate;
  // gameInfo->SetWhitePlayerName(PW);
  // gameInfo->SetBlackPlayerName(PB);
  // gameInfo->SetGameDates(SgfcDate::FromPropertyValue(DT));
  // game->GetRootNode()->WriteGameInfo(gameInfo);

  std::shared_ptr<ISgfcNode> node[vRecord.size()];

  auto builder = game->GetTreeBuilder();

  auto propertyValueFactory = SgfcPlusPlusFactory::CreatePropertyValueFactory();
  std::shared_ptr<ISgfcPropertyValue> propertyValueGM = propertyValueFactory->CreateNumberPropertyValue(4);
  std::shared_ptr<ISgfcPropertyValue> propertyValueSZ = propertyValueFactory->CreateNumberPropertyValue(bSize);
  auto propertyValueDT = propertyValueFactory->CreateSimpleTextPropertyValue(sDate.empty()?"":sDate);
  auto propertyValuePB = propertyValueFactory->CreateSimpleTextPropertyValue(sBName);
  auto propertyValuePW = propertyValueFactory->CreateSimpleTextPropertyValue(sWName);

  auto propertyFactory = SgfcPlusPlusFactory::CreatePropertyFactory();
  std::shared_ptr<ISgfcProperty> propertyGM = propertyFactory->CreateProperty(SgfcPropertyType::GM, propertyValueGM);
  std::shared_ptr<ISgfcProperty> propertySZ = propertyFactory->CreateProperty(SgfcPropertyType::SZ, propertyValueSZ);
  std::shared_ptr<ISgfcProperty> propertyDT = propertyFactory->CreateProperty(SgfcPropertyType::DT, propertyValueDT);
  std::shared_ptr<ISgfcProperty> propertyPB = propertyFactory->CreateProperty(SgfcPropertyType::PB, propertyValuePB);
  std::shared_ptr<ISgfcProperty> propertyPW = propertyFactory->CreateProperty(SgfcPropertyType::PW, propertyValuePW);

  rootNode->SetProperties(std::vector<std::shared_ptr<ISgfcProperty>>{propertyGM, propertySZ, propertyDT, propertyPB, propertyPW});

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

int SGFOption::loadSgf(const std::string &inputFilePath, std::vector<std::pair<int, int>> &vRecord, unsigned int *bSize)
{
  int iFlag = -1;
  Board bTmp;
  vRecord.clear();
  std::shared_ptr<ISgfcDocument> document = ReadDocument(inputFilePath, true);
  if (document->IsEmpty())
  {
    return iFlag;
  }

  std::shared_ptr<ISgfcGame> game = document->GetGame();
  if (SgfcGameType::GomokuAndRenju != game->GetGameType())
  {
    return iFlag;
  }

  std::shared_ptr<ISgfcNode> rootNode = game->GetRootNode();
  if (nullptr != rootNode)
  {
    if (rootNode->HasProperties())
    {
      std::shared_ptr<ISgfcProperty> property;
      property = rootNode->GetProperty(SgfcPropertyType::SZ);
      if (nullptr != property && property->HasPropertyValues())
      {
        std::string s_size = property->GetPropertyValue()->ToSingleValue()->GetRawValue();
        *bSize = (unsigned int)std::stoi(s_size);
      }
    }
    std::shared_ptr<ISgfcNode> currentNode;
    currentNode = rootNode;

    while (currentNode->HasChildren())
    {
      currentNode = currentNode->GetFirstChild();
      if (currentNode->HasProperties())
      {
        std::shared_ptr<ISgfcProperty> property;
        property = currentNode->GetProperty(SgfcPropertyType::W);
        if (nullptr == property)
        {
          property = currentNode->GetProperty(SgfcPropertyType::B);
        }
        if (nullptr != property && property->HasPropertyValues())
        {
          std::string sTmp = property->GetPropertyValue()->ToSingleValue()->GetRawValue();
          int coord = bTmp.idx2Coord((s_2_idx(sTmp)));
          int color = SgfcPropertyType::B == property->GetPropertyType() ? STONECOLOR::BLACK : STONECOLOR::WHITE;
          std::pair<int, int> pTmp(coord, color);
          vRecord.push_back(pTmp);
        }
      }
    }
  }
  else
  {
    return iFlag;
  }
  iFlag = 0;

  return iFlag;
}
