/*************************************************************************** 
This is to certify that this project is my own work, based on my personal efforts in studying and 
applying the concepts learned. I have constructed the functions and their respective algorithms 
and corresponding code by myself. The program was run, tested, and debugged by my own 
efforts. I further certify that I have not copied in part or whole or otherwise plagiarized the 
work of other students and/or persons.
                                 Maria Monica Manlises, DLSU ID# 121XXXXX
***************************************************************************/

/* Description: 
   Programmed by: Manlises, Maria Monica
                  [S21]
   Last Modified: Feb. 3, 2021
   Version:       5.3
   Acknowledgements: https://devdocs.io/c/ 
*/
   
#include<stdio.h>
#include<math.h>

// items have name, price, and QTY
typedef struct
{
   char* sName;
   float fPrice;
   int nQty;
}
item;

// Function Prototypes
void
initializations(item crust_size[6], 
                item toppings[5],
                item cheeses[4], 
                item stuffings[3]);
void
initializeQty(item crust_size[6], 
              item toppings[5],
              item cheeses[4],
              item stuffings[3]);
void
startMessage();
void
getCrust(int* pCrust, 
         int* pMaintenance);
void
getSize(char* pSize, 
        int nCrust, 
        float* pTotal, 
        item crust_size[6], 
        int* pCancel);
void
getToppings(item toppings[5], 
            char cSize, 
            int* pCancel, 
            float* pTotal);
void
getCheese(int nQty, 
           item cheeses[4], 
           int* pCancel, 
           float fTotal);
void
getStuffing(item stuffings[3], 
            int* pCancel, 
            float* pTotal);
void
payment(float fTotal, 
        int* pCancel);
void
receipt(item crust_size[6], 
        item toppings[5], 
        item cheeses[4], 
        item stuffings[3], 
        float fTotal, 
        FILE* pReceipts, 
        int nInvoice);
int
calcTime(item toppings[5], 
              item stuffings[3]);
void
creation(int nTime);


char toUpper(char c);
void separator();
void displayTotal(float fTotal);

int
main()
{
   // Declaration of arrays of items
   item crust_size[6];
   item toppings[5];
   item cheeses[4];
   item stuffings[3];

   // Cancel: 1 - Cancel; 0 - Continue
   int nCancel;
   // Crust: 1 - Thin; 2 - Thick
   int nCrust;
   // Size: S, M, L
   char cSize;
   // Total
   float fTotal;
   // Maintenance
   int nMaintenance = 0;
   // File pointer writes over txt file
   /* Note: All transactions of a single 
      run of the program will be saved in 
      Receipts.txt. BUT each run 
      overwrites the file. 
   */
   FILE* pReceipts = fopen("Receipts.txt", "w");
   // Invoice
   int nInvoice = 1;

   // Initialize struct arrays
   initializations(crust_size, 
                   toppings, 
                   cheeses, 
                   stuffings);

   // Loops till cued to maintenance
   while(!nMaintenance)
   {   
      // Initialize needed per order
      fTotal = 0;
      nCancel = 0;

      initializeQty(crust_size,
                    toppings,
                    cheeses,
                    stuffings);
      
      /* Set to "a" to only append after 
         each order */
      pReceipts = fopen("Receipts.txt", "a");
      
      // Introductory message
      startMessage();
      separator();

      // |STEP 1|: CRUST
      getCrust(&nCrust,
               &nMaintenance);
      separator();

      /* Proceed if not cued to go 
         under maintenance */
      if (!nMaintenance)
      {
         // |STEP 2|: SIZE
         getSize(&cSize,
                 nCrust,
                 &fTotal,
                 crust_size,
                 &nCancel);
         separator();
         
         if (!nCancel)
         {
            // |STEP 3|: TOPPINGS
            getToppings(toppings,
                        cSize,
                        &nCancel,
                        &fTotal);
            separator();

            // If chose cheese topping
            if(!nCancel && toppings[3].nQty > 0)
            {
               // |CHEESE|
               getCheese(toppings[3].nQty,
                         cheeses,
                         &nCancel,
                         fTotal);
               separator();
            }        
            
            // If thick crust
            if(!nCancel && nCrust == 2)
            {
               // |STUFFING|
               getStuffing(stuffings,
                           &nCancel,
                           &fTotal);
               separator();
            }

            if (!nCancel)
            {
               // |PAYMENT|
               payment(fTotal,
                       &nCancel);
               separator();

               if (!nCancel)
               {
                  // |RECEIPT|
                  receipt(crust_size,
                          toppings,
                          cheeses,
                          stuffings,
                          fTotal,
                          pReceipts,
                          nInvoice);
                  separator();

                  // |CREATION|
                  creation(calcTime(toppings, 
                              stuffings));
                  // increase invoice number
                  nInvoice++;
                  /* close the FILE pointer so
                     that it already saves the
                     data in the file after each
                     transaction
                  */
                  fclose(pReceipts);
               }
            }
         }
         
      }

      if (nCancel)
      {
         printf("Order has been ");
         printf("CANCELLED\n");
      }
      
   }

   printf("Machine will be shut down ");
   printf("for MAINTENANCE.\n");
   separator();
   return 0;
}

/* This function displays an introductory
   message
*/
void startMessage()
{
   separator();
   printf("Welcome to Easy Pizza!\n");
   printf("All our pizzas already "); 
   printf("include a tomato\n");
   printf("  sauce base and ");
   printf("mozzarella cheese\n");
   printf("Start ordering: \n");
}

/* This function initializes the constant
   data items of the crust_size, toppings,
   cheeses, and stuffings arrays such as 
   the names and prices.
   @param crust_size[6] is the array of 
      available crust_size items
   @param toppings[5] is the array of
      available topping items
   @param cheeses[4] is the array of
      available cheese items
   @param stuffings[3] is the array of
      available stuffing items
*/
void initializations(item crust_size[6],
                     item toppings[5],
                     item cheeses[4],
                     item stuffings[3])
{
   // loop counter
   int i;
   
   //crust_size
   char* sNames[6] = {"Thin Crust [S]",
                      "Thin Crust [M]",
                      "Thin Crust [L]",
                      "Thick Crust [S]",
                      "Thick Crust [M]",
                      "Thick Crust [L]"};
   float fPrices[3] = {100.00, 
                       175.00, 
                       225.00};
   for(i = 0; i < 6; i++)
   {
      crust_size[i].sName = sNames[i];
      crust_size[i].fPrice = fPrices[i % 3];
   }

   //toppings
   /* prices vary per order so 
      initialization happens in loop*/
   char* toppingNames[5] = {"Ham",
                            "Pineapple",
                            "Sausage",
                            "Cheese",
                            "Olives"};
   for (i = 0; i < 5; i++)
      toppings[i].sName = toppingNames[i];
   

   //cheeses
   // prices are not needed
   char* cheeseNames[4] = {"Mozzarella",
                           "Gorgonzola",
                           "Fontina",
                           "Parmesan"};
   for (i = 0; i < 4; i++)
      cheeses[i].sName = cheeseNames[i];

   //stuffings
   char* stuffingNames[3] = {"Cheese", 
                             "Spam",
                             "Both C&S"};
   float stuffingPrices[3] = {30.00,
                              25.00,
                              35.00};
   for (i = 0; i < 3; i++)
   {
      stuffings[i].sName = stuffingNames[i];
      stuffings[i].fPrice = stuffingPrices[i];
   }
}

/* This function initializes the qtys of 
   the items to 0
   @param crust_size[6] is the array of 
      available crust_size items
   @param toppings[5] is the array of
      available topping items
   @param cheeses[4] is the array of
      available cheese items
   @param stuffings[3] is the array of
      available stuffing items
*/
void initializeQty(item crust_size[6],
                   item toppings[5],
                   item cheeses[4],
                   item stuffings[3])
{
   int i;
   for (i = 0; i < 6; i++)
   {
      crust_size[i].nQty = 0;
      if (i < 5)
         toppings[i].nQty = 0;
      if (i < 4)
         cheeses[i].nQty = 0;
      if (i < 3)
         stuffings[i].nQty = 0;
   }
}

/* This function does STEP 1, which asks
   the user to choose the type of crust:
   (1) thin crust or (2) thick crust
   @param pCrust is the pointer to the
      memory where the crust is stored
   @param pMaintenance is the pointer
      to the memory where the variable
      maintenance is stored
*/
void getCrust(int* pCrust,
              int* pMaintenance)
{
   // input variable
   int nInput;
   
   printf("|STEP 1|\n");
   // display available options
   printf("Choose your crust:\n");
   printf(" [1] - Thin Crust\n");
   printf(" [2] - Thick Crust\n");
   
   // loop to check if input is valid
   do
   {
      printf("\nCrust: ");
      scanf("%d", &nInput);
         
      // check for invalid inputs
      if(!(nInput == 1 || nInput == 2 || 
           nInput == 80808))
      {
         printf("Invalid Input. ");
         printf("Choose a valid crust.\n");
         nInput = -1;
      }
   } while(nInput == -1);

   // maintenance code
   if(nInput == 80808)
      *pMaintenance = 1;
   // save input to crust if valid
   else
      *pCrust = nInput;
}

/* This function does STEP 2, which asks
   the user to input the size of the pizza
   S - small, M - medium, L - large
   Preconditions: nCrust is 1 or 2;
      crust_size[6] has been initialized
   @param pSize points to the memory where
      the size variable is stored
   @param nCrust is the type of crust
      which will be used for the
      crust_size array
   @param pTotal points to the memory
      where the total is stored
   @param crust_size[6] is an array of 
      item structs of the available
      crust_size options
   @param pCancel points to the memory
      where the cancel variable, that
      determines cancellation, is stored
*/
void getSize(char* pSize,
             int nCrust,
             float* pTotal,
             item crust_size[6],
             int* pCancel)
{
   // input variable
   char cInput;
   
   // index for crust_size based on crust
   int nIndex = (nCrust - 1) * 3;

   printf("|STEP 2|\n");
   // display available options
   printf("Choose a size:\n");
   printf(" [S] - Small     \t");
   printf("(PhP %.2f)\n", 
            crust_size[0].fPrice);
   printf(" [M] - Medium     \t");
   printf("(PhP %.2f)\n", 
            crust_size[1].fPrice);
   printf(" [L] - Large     \t");
   printf("(PhP %.2f)\n", 
            crust_size[2].fPrice);
   printf(" [X] - CANCEL ORDER\n\n");
   
   // loops until input is valid
   do
   {
      printf("Size: ");
      scanf(" %c", &cInput);
      // capitalizes input
      cInput = toUpper(cInput);

      switch(cInput)
      {
         // store accordingly if valid
         case 'L': nIndex++;
         case 'M': nIndex++;
         case 'S': *pSize = cInput;
                   crust_size[nIndex].nQty = 1;
                   *pTotal += crust_size[nIndex].fPrice;
                   displayTotal(*pTotal);
                   break;
         // cancel
         case 'X': *pCancel = 1;
                   break;
         // invalid
         default: printf("Invalid Input. ");
                  printf("Choose a valid size.\n");
                  cInput = '\0';
                  break;
      }  
   } while(cInput == '\0');
}

/* This function does STEP 3, which asks
   the user to input the toppings
   Preconditions: cSize is S, M, or L;
      toppings[5] has been initialized
   @param toppings[5] is the array of 
      available topping items
   @param cSize is the size which changes
      the prices of the toppings
   @param pCancel points to the memory
      where the cancel variable, that
      determines cancellation, is stored
   @param pTotal points to the memory
      where the total is stored
*/
void getToppings(item toppings[5],
                 char cSize,
                 int* pCancel,
                 float* pTotal)
{
   // base prices of toppings
   float basePrices[5] = {10.00,
                          6.00,
                          15.00,
                          12.00,
                          10.00};
   // multiplier depending on size
   float fPriceMultiplier;
   // input variable
   char cInput;
   // count of toppings
   int nCount = 0;
   // loop counter
   int i;
   // index to store chosen topping
   int nIndex;

   // *****PART 1***** (for test script)
   //change price multiplier based on size
   switch(cSize)
   {
      case 'S':   fPriceMultiplier = 1;
                  break;
      case 'M':   fPriceMultiplier = 1.5;
                  break;
      case 'L':   fPriceMultiplier = 1.75;
                  break;
   }

   // store appropriate price per topping
   for(i = 0; i < 5; i++)
      toppings[i].fPrice = 
         basePrices[i] * fPriceMultiplier;

   printf("|STEP 3|\n");
   printf("Choose at least 1 topping:\n");
   
   // loop to display the options
   for(i = 0; i < 5; i++)
   {
      printf(" [%c] - %s", 
               toppings[i].sName[0],
               toppings[i].sName);
      printf("      \t");
      printf("(PhP %.2f)\n",
               toppings[i].fPrice);
   }
   printf(" [N] - NEXT STEP\n");
   printf(" [X] - CANCEL ORDER\n\n");

   // *****PART 2***** (for test script)
   // loop till next or cancel is selected
   do
   {
      printf("Topping # %d: ", 
               nCount + 1);
      scanf(" %c", &cInput);
      // capitalize input
      cInput = toUpper(cInput);

      nIndex = 0;

      switch(cInput)
      {
         // store accordingly if valid
         case 'O': nIndex++;
         case 'C': nIndex++;
         case 'S': nIndex++;
         case 'P': nIndex++;
         case 'H': toppings[nIndex].nQty++;
                   *pTotal += toppings[nIndex].fPrice;
                   nCount++;
                   displayTotal(*pTotal);
                   break;
         // next
         case 'N': if (nCount == 0)
                   {
                     printf("You must select at least 1 topping.\n");
                     cInput = '\0';
                   }
                   break;
         // cancel
         case 'X': *pCancel = 1;
                   break;
         // invalid
         default:  printf("Invalid Input. ");
                   printf("Choose a valid topping.\n");
                   break;
      }
   } while (cInput != 'N' && cInput != 'X');
}

/* This function does STEP 4, which asks
   the user to select the type of cheese
   for each cheese topping they ordered
   Precondition: nQty is positive;
      cheeses[4] has been initialized
   @param nQty is the number of cheeses
      ordered
   @param cheeses[4] is the array of 
      available cheese items
   @param pCancel points to the memory
      where the cancel variable, that
      determines cancellation, is stored
   @param pTotal points to the memory
      where the total is stored
*/
void getCheese(int nQty,
               item cheeses[4],
               int* pCancel,
               float fTotal)
{
   // input variable
   char cInput;
   // loop counter
   int i;
   // index of chosen cheese type
   int nIndex;

   printf("|CHEESE|\n");
   printf("You chose %d ", nQty);
   printf("cheese toppings.\n");
   printf("Select what type of cheese");
   printf(" for each:\n");
   
   // loop to display available cheeses
   for (i = 0; i < 4; i++)
   {
      printf(" [%c] - %s\n", 
               cheeses[i].sName[0],
               cheeses[i].sName);
   }
   printf(" [X] - CANCEL ORDER\n\n");

   // loop to choose cheese
   for (i = 1; i <= nQty; i++)
   {
      printf("Cheese # %d: ", i);
      scanf(" %c", &cInput);
      
      // capitalize input
      cInput = toUpper(cInput);

      nIndex = 0;
      switch(cInput)
      {
         // store accordingly if valid
         case 'P': nIndex++;
         case 'F': nIndex++;
         case 'G': nIndex++;  
         case 'M': cheeses[nIndex].nQty++;
                   break;
         // cancel
         case 'X': *pCancel = 1;
                   i = nQty;
                   break;
         // invalid
         default:  printf("Invalid Input. ");
                   printf("Choose a valid cheese type.\n");
                   i--;
                   break;
      }
   }
   if (*pCancel != 1)
      displayTotal(fTotal);
}

/* This function does STEP 5, which asks
   the user to select the stuffing if
   they chose thick crust
   Precondition: stuffings[3] has been 
      initialized
   @param stuffings[3] is the array of 
      available stuffing items
   @param pCancel points to the memory
      where the cancel variable, that
      determines cancellation, is stored
   @param pTotal points to the memory
      where the total is stored
*/
void getStuffing(item stuffings[3],
                  int* pCancel,
                  float* pTotal)
{
   // input variable
   char cInput;
   // loop counter
   int i;
   // index of chosen stuffing
   int nIndex;

   printf("|STUFFING|\n");
   printf("Thick crust pizzas may have stuffings.\n");
   printf("Choose your stuffing: \n");

   // loop to display available stuffings
   for(i = 0; i < 3; i++)
   {
      printf(" [%c] - %s     \t", 
               stuffings[i].sName[0], 
               stuffings[i].sName);
      printf("(PhP %.2f)\n", 
               stuffings[i].fPrice);
   }
   printf(" [N] - No Stuffing\n");
   printf(" [X] - CANCEL ORDER\n\n");
   
   do
   {
      printf("Stuffing: ");
      scanf(" %c", &cInput);
      // capitalizes Input
      cInput = toUpper(cInput);

      nIndex = 0;
      switch(cInput)
      {
         // store accordingly if valid
         case 'B': nIndex++;
         case 'S': nIndex++;
         case 'C': stuffings[nIndex].nQty++;
                   *pTotal += stuffings[nIndex].fPrice;
         // no topping
         case 'N': displayTotal(*pTotal);
                   break;
         // cancel
         case 'X': *pCancel = 1;
                   break;
         // invalid
         default:  printf("Invalid Input. ");
                   printf("Choose a valid stuffing.\n");
                   cInput = '\0';
                   break;
      }
   } while(cInput == '\0');
}

/* This function does STEP 6, which asks
   the user to input payment
   Precondition: fTotal is positive
   @param fTotal is the total amount to
      be paid by the user
   @param pCancel points to the memory
      where the cancel variable, that
      determines cancellation, is stored
*/
void payment(float fTotal,
               int* pCancel)
{
   // list of payment options
   float money[12] = {1000.0,
                        500.0, 
                        100.0, 
                        50.0, 
                        20.0, 
                        10.0, 
                        5.0, 
                        1.0, 
                        0.25, 
                        0.10, 
                        0.05, 
                        0.01};
   // loop counter
   int i;
   // input variable
   int nInput;
   // count for change
   int nCount;
   
   printf("|PAYMENT|\n");
   printf("Your TOTAL is Php %.2f\n", 
            fTotal);
   printf("\nPress the respective keys to pay:\n");

   // displays list of payment options
   // 2 columns
   for (i = 0; i < 6; i++)
   {
      printf("  [%d] - ", i + 1);
      printf("%.2f\t", money[i]);
      
      printf("  [%d] - ", i + 7);
      printf("%.2f\n", money[i + 6]);
   }
   // right-aligned cancel order
   printf("%18s[0] - CANCEL ORDER\n\n", " ");

   // loop for user to input payment
   while (fTotal > 0)
   {
      printf("Pay: ");
      scanf("%d", &nInput);

      // cancel
      if (nInput == 0)
      {
         fTotal = 0;
         *pCancel = 1;
      }
      // valid (non-decimal)
      else if (nInput >= 1 && nInput <= 8)
      {
         // subtract payment from total
         fTotal -= money[nInput - 1];
         // display remaining total
         displayTotal(fTotal);
      }
      // valid (with decimal)
      else if (nInput >= 9 && nInput <= 12)
      {
         // to avoid C float inaccuracies
         fTotal -= roundf(money[nInput - 1] * 100) / 100;
         // display remaining total
         displayTotal(fTotal);
      }
      // invalid
      else
      {
         printf("Invalid Input.\n");
         printf("Enter Keys from 0-12 only.\n");
      }
   }

   // Change:
   if (fTotal < 0)
   {
      printf("Change: \n");
      // to avoid C float inaccuracies
      fTotal = roundf(fTotal * -100);
      
      // minimum bills/coins
      for (i = 0; i < 12; i++)
      {
         nCount = 0;
         while(fTotal >= roundf(money[i] * 100))
         {
            fTotal -= roundf(money[i] * 100);
            nCount ++;
         }
         
         // display in words
         if (nCount > 0)
         {
            printf("  (%d) ", nCount);
            // bills (1000, 500, 200, 100, 50, 20)
            if (i < 5)
               printf("%.0f peso bill", 
                        money[i]);
            // Non-Centavo Coins (10, 5, 1)
            else if (i < 8)
               printf("%.0f peso coin", 
                        money[i]);
            // Centavo Coins
            else
               printf("%.0f centavo coin", 
                        money[i] * 100);
            // if more than 1
            if (nCount > 1)
               printf("s");
      
            printf("\n");
         }
      }
   }
}

//--------------------------------------
/* This function displays the receipt
      and stores this on a separate 
      Receipts.txt file
   Preconditions: (1) To retain screen 
      design, this assumes that all 
      Qtys are single-digit, but would 
      still work otherwise
      (2) fTotal, nInvoice are positive
      (3) pReceipts is set to append
      (4) all arr of item structs have
         been initialized properly
   @param crust_size[6] is the array of 
      available crust_size items
   @param toppings[5] is the array of
      available topping items
   @param cheeses[4] is the array of
      available cheese items
   @param stuffings[3] is the array of
      available stuffing items
   @param fTotal is the total amount to
      be paid by the user
   @param pReceipts is the pointer to the
      Receipts.txt file to be appended
   @param nInvoice is the invoice number
*/
void receipt(item crust_size[6], 
            item toppings[5], 
            item cheeses[4], 
            item stuffings[3], 
            float fTotal, 
            FILE* pReceipts, 
            int nInvoice)
{   
   // loop counters
   int i, j;
   
   printf("|RECEIPT|\n\n");
   // Display Invoice Number
   printf("Invoice Number: ");
   printf("%04d\n\n", nInvoice);     

   // Display header
   printf("%-25s", "ITEM");
   printf("QTY");
   printf("%12s\n", "PRICE");
   
   // Stores invoice
   fprintf(pReceipts, 
      "Invoice Number: ");
   fprintf(pReceipts, "%04d\n\n", 
      nInvoice);
   
   // Stores header
   fprintf(pReceipts, "%-25s", 
      "ITEM");
   fprintf(pReceipts, "QTY");
   fprintf(pReceipts, "%12s\n", 
      "PRICE");

   // Display and store crust and size
   for (i = 0; i < 6; i++)
   {
      if (crust_size[i].nQty == 1)
      {
         printf("%-25s", 
               crust_size[i].sName);
         printf(" 1 ");
         printf("%12.2f\n", 
               crust_size[i].fPrice);

         fprintf(pReceipts, "%-25s",
               crust_size[i].sName);
         fprintf(pReceipts, " 1 ");
         fprintf(pReceipts, 
               "%12.2f\n",
               crust_size[i].fPrice);
      }
   }

   // Display and store toppings ordered
   for (i = 0; i < 5; i++)
   {
      if(toppings[i].nQty > 0)
      {
         printf("%-25s", 
                  toppings[i].sName);
         printf(" %d ", 
                  toppings[i].nQty);
         printf("%12.2f\n", 
                  toppings[i].fPrice * 
                     toppings[i].nQty);

         fprintf(pReceipts, "%-25s", 
                  toppings[i].sName);
         fprintf(pReceipts, " %d ", 
                  toppings[i].nQty);
         fprintf(pReceipts, "%12.2f\n", 
                  toppings[i].fPrice * 
                     toppings[i].nQty);

         /* If there is cheese, 
            display and store the types
         */
         if (i == 3)
         {
            for (j = 0; j < 4; j++)
            {
               if (cheeses[j].nQty > 0)
               {
                  printf("   %s", 
                        cheeses[j].sName);
                  printf(" x %d\n", 
                        cheeses[j].nQty);

                  fprintf(pReceipts, "   %s", 
                        cheeses[j].sName);
                  fprintf(pReceipts, " x %d\n", 
                        cheeses[j].nQty);
               }
            }
         }
      }
   }

   // Display and store stuffings ordered
   for (i = 0; i < 3; i++)
   {
      if (stuffings[i].nQty > 0)
      {
         printf("Stuffing: ");
         printf("%-15s", 
                  stuffings[i].sName);

         printf(" 1 ");
         printf("%12.2f\n", 
                  stuffings[i].fPrice);

         fprintf(pReceipts, "%-25s", 
                  stuffings[i].sName);
         fprintf(pReceipts, " 1 ");
         fprintf(pReceipts, "%12.2f\n", 
                  stuffings[i].fPrice);
      }
   }
   
   // Display Total
   printf("\n");
   displayTotal(fTotal);
   // Store Total
   fprintf(pReceipts, "\n");
   fprintf(pReceipts, "%35s", "TOTAL: ");
   fprintf(pReceipts, "%.2f\n", fTotal);
   fprintf(pReceipts, "\n----------------------------------------\n\n");
}

/* This function calculates the amount
      of time it would take to create 
      the ordered pizza
   Precondition: toppings and stuffings
      are valid arrays of item structs
      with valid Qtys
   @param toppings is the list of avail.
      toppings; mainly the nQty data 
      will be used in this function
   @param stuffings is the list of avail.
      stuffings; mainly the nQty data 
      will be used in this function
   @return the calculted time (in seconds) 
      it would take to create the pizza 
      based on the number of toppings 
      and stuffings
*/
int 
calcTime(item toppings[5], 
            item stuffings[3])
{
   // The base amount of time is 3 mins
   int nTime = 180;
   
   // Number of toppings ordered
   int nTopCount = 0;
   
   //  Number of stuffings ordered
   int nStuffCount = 0;
   
   // Loop counter
   int i;
   
   // Loop to count the qty of toppings
   for(i = 0; i < 5; i ++)
      nTopCount += toppings[i].nQty;

   // Add 2 secs for each topping
   nTime += (nTopCount * 2);

   // Loop to count the qty of stuffings
   for(i = 0; i < 3; i++)
   {
      nStuffCount += stuffings[i].nQty;
      // If Both was chosen, stuff = 2
      if(i == 2 && stuffings[i].nQty == 1)
         nStuffCount++;
   }
   
   // Add 5 secs for each stuffing
   nTime += (nStuffCount * 5);

   return nTime;
}

/* This function simulates the creation
      of the pizza by showing a timer
      that counts down based on the
      calculated time 
   Precondition: nTime is a valid integer
   @param nTime is the calculated time it
      takes to create the ordered pizza
*/
void
creation(int nTime)
{
   int nCount = 1;
   printf("Creating Pizza...\n");
   printf("Please wait...\n");
   printf("Time left:\n");
   
   while(nTime >= 0)
   {
      printf("%d:%02d  ", 
               nTime / 60, 
               nTime % 60);
      nTime--;
      // next line after every 7th
      if (nCount % 7 == 0)
         printf("\n");
      nCount++;
   }

   printf("\nThank you for waiting!\n");
   printf("Here is your pizza!\n");
   printf("We hope you order again soon!\n");
}


/* This function prints dashes and 
   next line for screen design purposes
*/
void
separator()
{
   printf("\n----------------------------------------\n\n");
}

/* This function capitalizes the given 
      character if it is in lower case
   Precondition: c is a valid character
   @param c is the character to be capitalized
   @return the capitalized version of c
*/
char
toUpper(char c)
{
   if(c >= 'a' && c <= 'z')
      return c - 32;
   return c;
}


/* This function displays the total,
      right-aligned for screen design
      purposes
   Precondition: fTotal is a valid float
   @param fTotal is the amount to be paid
*/
void
displayTotal(float fTotal)
{
   printf("%33s", "TOTAL: ");
   printf("%.2f\n", fTotal);
}

