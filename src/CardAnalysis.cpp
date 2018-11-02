/*
 * CardAnalysis.cpp
 *
 *  Created on: Oct 28, 2018
 *      Author: milites
 */

#include <stdio.h>
#include <stdlib.h>
#include <assets/cards/CardLibrary.h>

using namespace std;

FILE *out = NULL;
#define UNIFIED_OUT 1
#if UNIFIED_OUT
#else
#define INDIVIDUAL_OUT
#endif

void printHeader()
{
   // Keywords
   fprintf(out,"Keywords\n");
   fprintf(out,"Front Load: Total cost required to get a card operating at max expense starting from the player's hand (includes 1 expense action)\n");
   fprintf(out,"Lifetime: Value achievable over the course of 7 rounds - Assuming a start in round 1\n");
   fprintf(out,"Instantaneous: Occurring within a single action\n");
   fprintf(out,"Hybrid: Only relevant for properties - an approach that combines expensing and depreciation in a manner that still maximizes depreciation\n");
   fprintf(out,"Efficiency: Money per unit cost\n");
   fprintf(out,"Cost: Cost without a modifier (e.g. Action Cost) refers to Money Cost\n");
   // Basic Properties
   fprintf(out,"Data\n");
   fprintf(out,"Id");
   fprintf(out,",Name");
   fprintf(out,",Type");
   fprintf(out,",Group");
   fprintf(out,",Deck");
   fprintf(out,",Play Action Cost");
   fprintf(out,",Expense Action Cost");
   fprintf(out,",Property Value Max");
   fprintf(out,",Property Value Min");
   // Depreciation Metrics
   fprintf(out,",Depreciation Count");
   fprintf(out,",Depreciation Rate Min");
   fprintf(out,",Depreciation Rate Max");
   fprintf(out,",Depreciation Lifetime Cost");
   fprintf(out,",Depreciation Front Load Action Cost");
   fprintf(out,",Depreciation Lifetime Action Cost");
   fprintf(out,",Depreciation Lifetime Action Efficiency");
   // Hybrid Metrics
   fprintf(out,",Hybrid Front Load Action Cost");
   fprintf(out,",Hybrid Front Load Companion Cost");
   fprintf(out,",Hybrid Front Load Companion Probability");
   fprintf(out,",Hybrid Lifetime Expense Cost");
   fprintf(out,",Hybrid Lifetime Action Cost");
   fprintf(out,",Hybrid Lifetime Cost");
   fprintf(out,",Hybrid Lifetime Action Efficiency");
   fprintf(out,",Hybrid Lifetime Companion Efficiency");
   // Expense Metrics
   fprintf(out,",Expense Upgrade Count");
   fprintf(out,",Expense Front Load Action Cost Min");
   fprintf(out,",Expense Front Load Action Cost Max");
   fprintf(out,",Expense Front Load Companion Cost Min");
   fprintf(out,",Expense Front Load Companion Cost Max");
   fprintf(out,",Expense Front Load Companion Probability Min");
   fprintf(out,",Expense Front Load Companion Probability Max");
   fprintf(out,",Expense Instantaneous Action Cost Min");
   fprintf(out,",Expense Instantaneous Action Cost Max");
   fprintf(out,",Expense Lifetime Action Cost Min");
   fprintf(out,",Expense Lifetime Action Cost Max");
   fprintf(out,",Expense Instantaneous Cost Min");
   fprintf(out,",Expense Instantaneous Cost Max");
   fprintf(out,",Expense Instantaneous Cost Avg");
   fprintf(out,",Expense Lifetime Cost Min");
   fprintf(out,",Expense Lifetime Cost Max");
   fprintf(out,",Expense Lifetime Cost Avg");
   fprintf(out,",Expense Instantaneous Action Efficiency Min");
   fprintf(out,",Expense Instantaneous Action Efficiency Max");
   fprintf(out,",Expense Lifetime Action Efficiency Min");
   fprintf(out,",Expense Lifetime Action Efficiency Max");
   // Effect Metrics
   fprintf(out,",Effect Front Load Action Cost");
   fprintf(out,",Effect Instantaneous Efficiency Gain");
   fprintf(out,",Effect Lifetime Efficiency Gain");
   fprintf(out,",Effect Single Card Lifetime Cost Gain");
   fprintf(out,",Effect Single Card Lifetime Efficiency");
   fprintf(out,",Effect Double Card Lifetime Efficiency");
   fprintf(out,",Effect Triple Card Lifetime Efficiency");

   fprintf(out,"\n");
}

void printDepreciationStats(Card::Ptr card)
{
   fprintf(out,"Depreciation Stats:\n");
   if (card->group == CardGroup::Property)
   {
      int min = 0;
      for (size_t i = 0; i < card->valueLevels.size(); ++i)
      {
         if (i+1 < card->valueLevels.size() &&
             (card->valueLevels.at(i).value - card->valueLevels.at(i+1).value < min || min == 0) )
         {
            min = card->valueLevels.at(i).value - card->valueLevels.at(i+1).value;
         }
      }
      fprintf(out,"  Min Rate: %d\n",min);

      int max = 0;
      for (size_t i = 0; i < card->valueLevels.size(); ++i)
      {
         if (i+1 < card->valueLevels.size() &&
             card->valueLevels.at(i).value - card->valueLevels.at(i+1).value > max)
         {
            max = card->valueLevels.at(i).value - card->valueLevels.at(i+1).value;
         }
      }
      fprintf(out,"  Max Rate: %d\n",max);

      int lifetime = card->valueLevels.at(0).value - card->valueLevels.at(card->valueLevels.size()-1).value;
      size_t num = card->valueLevels.size() - 1;
      float avg = num > 0 ? (float)lifetime / (float)num : 0.0f;
      fprintf(out,"  Avg Rate: %.02f\n",avg);
      fprintf(out,"  # of Depreciations: %lu\n",num);
      fprintf(out,"  Total Action Cost: 2\n");
      fprintf(out,"  Total Cost: %d\n",lifetime);

      float efficiency = (float) lifetime / 2.0f;
      fprintf(out,"  Action Efficiency: %.02f\n",efficiency);
   }
   else
   {
      fprintf(out,"  N/A\n");
   }
}

void printHybridDep(Card::Ptr card)
{
   fprintf(out,"Hybrid Depreciation Strategy:\n");
   if (card->group == CardGroup::Property)
   {
      int depreciation = card->valueLevels.at(0).value - card->valueLevels.at(card->valueLevels.size()-1).value;
      size_t numDep = card->valueLevels.size() - 1;
      size_t numExp = 1 + 6 - numDep; // Can always expense the first round
      size_t numUpg = card->upgradeLevels.size() - 1;
      int maxExp = card->upgradeLevels.at(numUpg).expense;
      int totalActions = 2 + numUpg + numExp;
      int totalCost = depreciation + maxExp * numExp;
      float efficiency = (float)totalCost / (float)totalActions;

      fprintf(out,"  # of Depreciations: %lu\n",numDep);
      fprintf(out,"  # of Upgrades: %lu\n",numUpg);
      fprintf(out,"  # of Expenses: %lu\n",numExp);
      fprintf(out,"  Max Expense: %d\n",maxExp);
      fprintf(out,"  Total Cost: %d\n",totalCost);
      fprintf(out,"  Total Action Cost: %d\n",totalActions);
      fprintf(out,"  Action Efficiency: %.02f\n",efficiency);
   }
   else
   {
      fprintf(out,"  N/A\n");
   }

}

void printBasicProperties(Card::Ptr card)
{
   fprintf(out,"%s",CardId::toString(card->id).c_str());
   fprintf(out,",%s",CardName::toString(card->name).c_str());
   fprintf(out,",%s",CardType::toString(card->type).c_str());
   fprintf(out,",%s",CardGroup::toString(card->group).c_str());
   fprintf(out,",%s",CardDeck::toString(card->deck).c_str());
   if (card->valueLevels.size() > 0 && card->group != CardGroup::Property)
      fprintf(out,",%d",card->valueLevels.at(0).value);
   else
      fprintf(out,",1");
   fprintf(out,",%d",card->expenseCost);
   if (card->group == CardGroup::Property)
   {
      fprintf(out,",%d",card->valueLevels.at(0).value);
      fprintf(out,",%d",card->valueLevels.at(card->valueLevels.size()-1).value);
   }
   else
   {
      fprintf(out,",0");
      fprintf(out,",0");
   }
}

void printDnHMetrics(Card::Ptr card)
{
   if (card->group == CardGroup::Property)
   {
      // Depreciation
      int dCount = card->valueLevels.size()-1;
      int dRateMin = dCount > 0 ? 1000 : 0;
      int dRateMax = 0;
      for (size_t i = 0; i < card->valueLevels.size()-1 && dCount > 0; ++i)
      {
         int rate = card->valueLevels.at(i).value - card->valueLevels.at(i+1).value;
         if (dRateMin > rate)
            dRateMin = rate;
         if (dRateMax < rate)
            dRateMax = rate;
      }
      int dLCost = card->valueLevels.at(0).value - card->valueLevels.at(dCount).value;
      int dFLACost = 1;
      int dLACost = 2;
      float dLAEfficiency = (float)dLCost / (float)dLACost;

      fprintf(out,",%d",dCount);
      fprintf(out,",%d",dRateMin);
      fprintf(out,",%d",dRateMax);
      fprintf(out,",%d",dLCost);
      fprintf(out,",%d",dFLACost);
      fprintf(out,",%d",dLACost);
      fprintf(out,",%.02f",dLAEfficiency);

      // Hybrid
      int hFLACost = card->upgradeLevels.size() + 1;
      int hFLCCost = 0;
      for (size_t i = 0; i < card->upgradeLevels.size(); ++i)
      {
         switch (card->upgradeLevels.at(i).cost)
         {
            case Resource::Dog:
            case Resource::Horse:
            case Resource::Chef:
            case Resource::Guest:
               hFLCCost++;
               break;
         }
      }
      float hFLCProbability = -666.666f;
      int numUpgrades = card->upgradeLevels.size() - 1;
      int numExpenses = 1 + 6 - dCount;
      int hLECost = numExpenses * card->expenseCost;
      int hLACost = 2 + hLECost + numUpgrades;
      int hLCost = card->upgradeLevels.at(numUpgrades).expense * numExpenses + dLCost;
      float hLAEfficiency = (float)hLCost / (float)hLACost;
      float hLCEfficiency = (float)hLCost / (float)hFLCCost;

      fprintf(out,",%d",hFLACost);
      fprintf(out,",%d",hFLCCost);
      fprintf(out,",%.04f",hFLCProbability);
      fprintf(out,",%d",hLECost);
      fprintf(out,",%d",hLACost);
      fprintf(out,",%d",hLCost);
      fprintf(out,",%.02f",hLAEfficiency);
      fprintf(out,",%.02f",hLCEfficiency);
   }
   else
   {
      fprintf(out,",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0");
   }
}

void printExpenseMetrics(Card::Ptr card)
{
   int numExpenses = 7;
   int eUCount = card->upgradeLevels.size() - 1;
   int eFLACostMin = -666;
   int eFLACostMax = -666;
   switch (card->group)
   {
      case CardGroup::Event:
         eFLACostMin = card->valueLevels.at(0).value;
         eFLACostMax = card->valueLevels.at(card->valueLevels.size()-1).value;
         break;
      case CardGroup::HelperExpense:
         eFLACostMin = card->valueLevels.at(0).value + card->expenseCost;
         eFLACostMax = card->valueLevels.at(0).value + eUCount + numExpenses * card->expenseCost;
         break;
      case CardGroup::Property:
         eFLACostMin = 2;
         eFLACostMax = 1 + eUCount + numExpenses * card->expenseCost;
         break;
      default:
         break;
   }
   int eFLCCostMin = 0;
   int eFLCCostMax = 0;
   for (size_t i = 0; i < card->upgradeLevels.size(); ++i)
   {
      switch (card->upgradeLevels.at(i).cost)
      {
         case Resource::Dog:
         case Resource::Horse:
         case Resource::Chef:
         case Resource::Guest:
            eFLCCostMax++;
            break;
      }
   }
   float eFLCProbabilityMin = -666.666f;
   float eFLCProbabilityMax = -666.666f;
   int eIACostMin = card->expenseCost;
   int eIACostMax = card->expenseCost;
   int eLACostMin = eFLACostMin + (numExpenses-1) * eIACostMin;
   int eLACostMax = eFLACostMax + (numExpenses-1) * eIACostMax;
   if (card->group == CardGroup::Event)
   {
      eIACostMin = card->valueLevels.at(0).value;
      eIACostMax = card->valueLevels.at(card->valueLevels.size()-1).value;
      eLACostMin = eFLACostMin;
      eLACostMax = eFLACostMax;
   }
   else if (card->group == CardGroup::Property)
   { // For the sell action at the end
      eLACostMin++;
      eLACostMax++;
   }
   int eICostMin = 0;
   int eICostMax = 0;
   float eICostAvg = 0.0f;
   int eLCostMin = 0;
   int eLCostMax = 0;
   float eLCostAvg = 0;
   if (card->upgradeLevels.size() > 0)
   {
      switch (card->group)
      {
         case CardGroup::Event:
            eICostAvg = -666.666f;
            eICostMin = card->upgradeLevels.at(0).expense;
            for (int i = 0; i <= eUCount; ++i)
               eICostMax += card->upgradeLevels.at(i).expense;
            eLCostAvg = -666.666f;
            eLCostMin = eICostMin;
            eLCostMax = eICostMax;
            break;
         case CardGroup::HelperExpense:
         case CardGroup::Property:
            eICostMin = card->upgradeLevels.at(0).expense;
            eICostMax = card->upgradeLevels.at(eUCount).expense;
            for (int i = 0; i <= eUCount; ++i)
               eICostAvg += (float)card->upgradeLevels.at(i).expense;
            eICostAvg /= (float)card->upgradeLevels.size();
            eLCostMin = eICostMin * numExpenses;
            eLCostMax = eICostMax * numExpenses;
            eLCostAvg = eICostAvg * numExpenses;
            break;
         default:
            break;
      }
   }
   float eIAEfficiencyMin = eIACostMin != 0 ? eICostMin / eIACostMin : 0.0f;
   float eIAEfficiencyMax = eIACostMax != 0 ? eICostMax / eIACostMax : 0.0f;
   float eLAEfficiencyMin = eLACostMin != 0 ? eLCostMin / eLACostMin : 0.0f;
   float eLAEfficiencyMax = eLACostMax != 0 ? eLCostMax / eLACostMax : 0.0f;

   fprintf(out,",%d",eUCount);
   fprintf(out,",%d",eFLACostMin);
   fprintf(out,",%d",eFLACostMax);
   fprintf(out,",%d",eFLCCostMin);
   fprintf(out,",%d",eFLCCostMax);
   fprintf(out,",%.04f",eFLCProbabilityMin);
   fprintf(out,",%.04f",eFLCProbabilityMax);
   fprintf(out,",%d",eIACostMin);
   fprintf(out,",%d",eIACostMax);
   fprintf(out,",%d",eLACostMin);
   fprintf(out,",%d",eLACostMax);
   fprintf(out,",%d",eICostMin);
   fprintf(out,",%d",eICostMax);
   fprintf(out,",%.02f",eICostAvg);
   fprintf(out,",%d",eLCostMin);
   fprintf(out,",%d",eLCostMax);
   fprintf(out,",%.02f",eLCostAvg);
   if (eIACostMin != 0)
      fprintf(out,",%.04f",eIAEfficiencyMin);
   else
      fprintf(out,",Inf");
   if (eIACostMax != 0)
      fprintf(out,",%.04f",eIAEfficiencyMax);
   else
      fprintf(out,",Inf");
   if (eLACostMin != 0)
      fprintf(out,",%.04f",eLAEfficiencyMin);
   else
      fprintf(out,",Inf");
   if (eLACostMax != 0)
      fprintf(out,",%.04f",eLAEfficiencyMax);
   else
      fprintf(out,",Inf");

}

void printAnalysis(Card::Ptr card)
{
   string id = CardId::toString(card->id);
#ifdef INDIVIDUAL_OUT
   string outFile = "analysis/" + id + ".txt";
   out = fopen(outFile.c_str(),"w+");
   if (out == NULL)
   {
      fprintf(stderr,"Failed to open \"%s\"\n",outFile.c_str());
      return;
   }
#endif

//   fprintf(out,"Id:    %s\n",id.c_str());
//   fprintf(out,"Name:  %s\n",CardName::toString(card->name).c_str());
//   fprintf(out,"Group: %s\n",CardGroup::toString(card->group).c_str());
//   fprintf(out,"Type:  %s\n",CardType::toString(card->type).c_str());
//   fprintf(out,"Deck:  %s\n",CardDeck::toString(card->deck).c_str());
//   fprintf(out,"Expense Cost: %d\n",card->expenseCost);
//   fprintf(out,"Value Levels:\n");
//   for (size_t i = 0; i < card->valueLevels.size(); ++i)
//      fprintf(out,"  %s : %d\n",
//         Resource::toString(card->valueLevels[i].resource).c_str(),
//         card->valueLevels[i].value);
//   fprintf(out,"Upgrade Levels:\n");
//   for (size_t i = 0; i < card->upgradeLevels.size(); ++i)
//      fprintf(out,"  %s : %d\n",
//         Resource::toString(card->upgradeLevels[i].cost).c_str(),
//         card->upgradeLevels[i].expense);
//
//   // Depreciation Stats
//   printDepreciationStats(card);
//
//   // Hybrid Depreciation Strategy
//   // Maximized depreciations and expensing when available
//   printHybridDep(card);
//
//   // Simple optimized expensing stats
//   // Does not include market/other card effects

   printBasicProperties(card);
   printDnHMetrics(card);
   printExpenseMetrics(card);

   fprintf(out,"\n");

#ifdef INDIVIDUAL_OUT
   fclose(out);
   out = NULL;
#endif
}

int main(int argc, char **argv)
{
#if UNIFIED_OUT
   string outFile = "analysis/unified.txt";
   out = fopen("analysis/unified.txt","w+");
   if (out == NULL)
   {
      fprintf(stderr,"Failed to open \"analysis/unified.txt\"\n");
      return 0;
   }
#endif

   printHeader();

   for (int i = CardId::Farm_1; i <= CardId::Steward_S2; ++i)
   {
      printAnalysis(CardLibrary::getInstance()->getCard(((CardIdType)i)));
   }

#if UNIFIED_OUT
   fclose(out);
#endif
   return 0;
}
