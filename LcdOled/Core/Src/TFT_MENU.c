/*
 * TFT_MENU.c
 *
 *  Created on: Nov 13, 2020
 *      Author: meh
 */

#include "TFT_MENU.h"
#include "stm32f4xx_hal.h"
#include "ST7735.h"
#include "fonts.h"
#include "GFX_FUNCTIONS.h"

// Menu buttons defines
int homemenu=0,page2menu=0,testmenu=0,testbutton=0,page2button=0,backbutton=0,ledonbutton=0,ledoffbutton=0;

// physical buttons defines
int downbutton=0, upbutton=0, enter=0;

// "TEST ALL" button on home (unselected)
void Test_Button_unselect (void)
{
	drawRoundRect(29, 19, 102, 27, 8, WHITE);
	fillRoundRect(30, 20, 100, 25, 8, RED);
	ST7735_WriteString(38, 23, "TEST ALL", Font_11x18, WHITE, RED);
	testbutton = 0;  // TEST ALL button is not selected
}

// "TEST ALL" button on home (selected)
void Test_Button_select (void)
{
	drawRoundRect(29, 19, 102, 27, 8, RED);
	fillRoundRect(30, 20, 100, 25, 8, GREEN);
	ST7735_WriteString(38, 23, "TEST ALL", Font_11x18, BLACK, GREEN);
	testbutton = 1;  // TEST ALL button is selected
}

// "PAGE 2" button on home (unselected)
void Page2_Button_unselect (void)
{
	drawRoundRect(29, 69, 102, 27, 8, WHITE);
	fillRoundRect(30, 70, 100, 25, 8, RED);
	ST7735_WriteString(40, 73, "PAGE 2", Font_11x18, WHITE, RED);
	page2button = 0;  // "PAGE 2" button is not selected
}

// "PAGE 2" button on home (selected)
void Page2_Button_select (void)
{
	drawRoundRect(29, 69, 102, 27, 8, RED);
	fillRoundRect(30, 70, 100, 25, 8, GREEN);
	ST7735_WriteString(40, 73, "PAGE 2", Font_11x18, BLACK, GREEN);
	page2button = 1;  // "PAGE 2" button is selected
}

// "GO BACK" button (unselected)
void back_Button_unselect (void)
{
	drawRoundRect(29, 89, 102, 27, 8, WHITE);
	fillRoundRect(30, 90, 100, 25, 8, RED);
	ST7735_WriteString(40, 93, "GO BACK", Font_11x18, WHITE, RED);
	backbutton = 0;  // "GO BACK" button is not selected
}

// "GO BACK" button (selected)
void back_Button_select (void)
{
	drawRoundRect(29, 89, 102, 27, 8, RED);
	fillRoundRect(30, 90, 100, 25, 8, GREEN);
	ST7735_WriteString(40, 93, "GO BACK", Font_11x18, BLACK, GREEN);
	backbutton = 1;  // "GO BACK" button is selected
}

// "LED ON" button (unselected)
void ledon_Button_unselect (void)
{
	drawRoundRect(29, 9, 102, 27, 8, WHITE);
	fillRoundRect(30, 10, 100, 25, 8, RED);
	ST7735_WriteString(38, 13, "LED ON", Font_11x18, WHITE, RED);
	ledonbutton = 0;  // "LED ON" button is not selected
}

// "LED ON" button (selected)
void ledon_Button_select (void)
{
	drawRoundRect(29, 9, 102, 27, 8, RED);
	fillRoundRect(30, 10, 100, 25, 8, GREEN);
	ST7735_WriteString(38, 13, "LED ON", Font_11x18, BLACK, GREEN);
	ledonbutton = 1;  // "LED ON" button is selected
}

// "LED OFF" button (unselected)
void ledoff_Button_unselect (void)
{
	drawRoundRect(29, 49, 102, 27, 8, WHITE);
	fillRoundRect(30, 50, 100, 25, 8, RED);
	ST7735_WriteString(38, 53, "LED OFF", Font_11x18, WHITE, RED);
	ledoffbutton = 0;  // "LED OFF" button is not selected
}

// "LED OFF" button (selected)
void ledoff_Button_select (void)
{
	drawRoundRect(29, 49, 102, 27, 8, RED);
	fillRoundRect(30, 50, 100, 25, 8, GREEN);
	ST7735_WriteString(38, 53, "LED OFF", Font_11x18, BLACK, GREEN);
	ledoffbutton = 1;  // "LED OFF" button is selected
}

/******Home Menu******/
void HomeMenu (void)
{
	ST7735_SetRotation(1);  // set horizontal
	fillScreen(BLACK);  // fill black
    Page2_Button_unselect();  // draw unselected "PAGE 2" button
	Test_Button_unselect();  // draw unselected "TEST ALL" button
	homemenu = 1;  // "HomeMenu" is selected
	page2menu = 0;  // "Page2Menu" is not selected
	testmenu = 0;  // "TestMenu" is not selected
}

/******Test Menu******/
void TestMenu (void)
{
	homemenu = 0;  // "HomeMenu" is not selected
	page2menu = 0;  // "Page2Menu is not selected
	testmenu = 1;  // "TestMenu" is selected

	testAll();  // Perform All the Tests

	ST7735_SetRotation(1);  // set Horizontal
	fillScreen(BLACK);  // fill black

	ST7735_WriteString(10, 20, "TEST COMPLETE", Font_11x18, GREEN, BLACK);  // Write "test completed"
	back_Button_unselect();  // draw unselected "GO BACK" button
}

/******Page 2  Menu******/
void Page2Menu (void)
{
	homemenu = 0;  // "HomeMenu" is not selected
	page2menu = 1;  // "Page2Menu" is selected
	testmenu = 0;  // "TestMenu" is not selected
	ST7735_SetRotation(1);  // set horizontal
	fillScreen(BLACK);  // fill black

	ledon_Button_unselect();  // draw unslected "LED ON" button
	ledoff_Button_unselect();  // draw unslected "LED OFF" button
	back_Button_unselect();  // draw unslected "GO BACK" button
}

/*******
 *  EXTI callback
 * PA0 is ENTER
 * PA1 is DOWN
 * PA4 is UP
 *******/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_0)  // If the PA0 (ENTER) is pressed
	{
		downbutton = 0;  // downbutton set to 0
		upbutton = 0;  // upbutton set to 0
		enter = 1;  // enter set to 1
	}

	if (GPIO_Pin == GPIO_PIN_1)  // If the PA1 (DOWN) is pressed
	{
		downbutton = 1;  // downbutton set to 1 and others set to 0
		upbutton = 0;
		enter = 0;
	}

	if (GPIO_Pin == GPIO_PIN_4)  // If the PA4 (UP) is pressed
	{
		downbutton = 0;
		upbutton = 1;  // upbutton set to 1 and others set to 0
		enter = 0;
	}
}

void Menu_Handler (void)
{
	  if (homemenu)  // If the Home Menu is called
	  {
		  HomeMenu();  // Draw the Home Menu
		  while (homemenu)  // While the control is inside the Home Menu
		  {
		  if (downbutton)  // if the down button is pressed
		  {
			  HAL_Delay (200);  // wait for some time to avoid error click
			  if (testbutton)  // if the "TEST ALL" button is already selected
			  {
				  Page2_Button_select();  // draw selected "PAGE 2" button
				  Test_Button_unselect(); // draw unselected "TEST ALL" button
				  downbutton = 0;  // reset the down button or else it will keep selecting in a loop
				  testbutton = 0;  // reset the test button also
			  }

			  else if (page2button)  // If the "PAGE 2" button is selected
			  {
				  Page2_Button_unselect();  // draw unselected "PAGE 2" button
				  Test_Button_select();  // draw selected "TEST ALL" button
				  downbutton = 0;  // reset the down button or else it will keep selecting in a loop
				  page2button = 0;  // reset the page 2 button
			  }

			  else  // If none of the buttons are selected
			  {
				  Page2_Button_unselect();  // draw unselected "PAGE 2" button
				  Test_Button_select();  // draw selected "TEST ALL" button
				  downbutton = 0;  // reset the down button or else it will keep selecting in a loop
			  }
		  }

		  else if (upbutton) // if the UP button is pressed
		  {
			  HAL_Delay (200);
			  if (testbutton)  // If the "TEST ALL" is already selected
			  {
				  // select the "PAGE 2" button and unselect the "TSET ALL" button
				  Page2_Button_select();
				  Test_Button_unselect();
				  upbutton = 0;
				  testbutton = 0;
			  }

			  else if (page2button)  // If the "PAGE 2" button is already selected
			  {
				  // select the "TEST ALL" button and unselect the "PAGE 2" button
				  Page2_Button_unselect();
				  Test_Button_select();
				  upbutton = 0;
				  page2button = 0;
			  }

			  else  // if none of the buttons are selected
			  {
				  // select the "PAGE 2" button and unselect the "TEST ALL" button
				  Page2_Button_select();
				  Test_Button_unselect();
				  upbutton = 0;
			  }
		  }
		  else if (enter)  // if the ENTER button is pressed
		  {
			  if (testbutton)  // if the "TEST ALL" button is already pressed
			  {
				  testmenu = 1;  // Select the TestMenu
				  homemenu = 0;  // Reset the HomeMenu
				  enter = 0;  // reset the enter button or else it will keep selecting in a loop
				  testbutton = 0;  // reset the down button
			  }

			  else if (page2button)  // if the "PAGE 2" button is already pressed
			  {
				  page2menu = 1;  // Select the Page2Menu
				  homemenu = 0;  // Reset the HomeMenu
				  enter = 0;  // reset the enter button or else it will keep selecting in a loop
				  page2button = 0;  // reset the up button
			  }

			  else;  // if none of the buttons are selected, then do nothing
		  }
		  else ;
	  }
	  }

	  else if (testmenu)  // If the Test Menu is called
	  {
		  TestMenu();  // Display the Test Menu
		  while (testmenu)  // While the control is in the Test Menu
		  {
			  if (downbutton || upbutton)  // Since there is only a Back Button, so either up or down pressed, it will result the same
			  {
				  HAL_Delay (200);
				  back_Button_select();  // Draw selected Back button
				  downbutton=0;
				  upbutton = 0;
			  }

			  if (enter)  // If the enter is pressed
			  {
				  if (backbutton)  // If the Back button is selected
				  {
					  homemenu = 1;  // Go back to the home menu
					  testmenu = 0;  // Reset every other menu
					  backbutton = 0;
					  enter = 0;
				  }
				  else ;
			  }

			  else ;
		  }
	  }

	  else if (page2menu)  // If the Page 2 Menu is called
	  {
		  Page2Menu();  // Display the Page 2 Menu
		  while (page2menu)  // While the control is in the Page Menu
		  {
			  if (downbutton)  // If down button is pressed
			  {
				  HAL_Delay (200);
				  if (ledonbutton)  // If the "LED ON" is selected
				  {
					  ledon_Button_unselect();  // Unselect "LED ON"
					  back_Button_unselect();  // unselect "GO BACK"
					  ledoff_Button_select();  // select "LED OFF"
					  downbutton = 0;  // Reset the down button
				  }

				  else if (ledoffbutton)
				  {
					  ledoff_Button_unselect();
					  back_Button_select();
					  ledon_Button_unselect();
					  downbutton = 0;
				  }

				  else if (backbutton)
				  {
					  ledoff_Button_unselect();
					  back_Button_unselect();
					  ledon_Button_select();
					  downbutton = 0;
				  }

				  else  // If none of the buttons were selected
				  {
					  // Select the "LED ON" button
					  ledoff_Button_unselect();
					  back_Button_unselect();
					  ledon_Button_select();
					  downbutton = 0;
				  }
			  }

			  // Similar to down button
			  else if (upbutton)
			  {
				  HAL_Delay (200);
				  if (ledonbutton)
				  {
					  ledon_Button_unselect();
					  back_Button_select();
					  ledoff_Button_unselect();
					  upbutton = 0;
				  }

				  else if (ledoffbutton)
				  {
					  ledoff_Button_unselect();
					  back_Button_unselect();
					  ledon_Button_select();
					  upbutton = 0;
				  }

				  else if (backbutton)
				  {
					  ledoff_Button_select();
					  back_Button_unselect();
					  ledon_Button_unselect();
					  upbutton = 0;
				  }

				  else
				  {
					  back_Button_select();
					  ledoff_Button_unselect();
					  ledon_Button_unselect();
					  upbutton = 0;
				  }
			  }

			  // If the enter is pressed
			  else if (enter)
			  {
				  if (ledonbutton)  // If "LED ON" is selected
				  {
					  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);  // Turn the LED ON
					  enter = 0;
				  }

				  else if (ledoffbutton)  // If "LED OFF" is selected
				  {
					  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);  // Turn the LED OFF
					  enter = 0;
				  }

				  else if (backbutton)  // If "GO BACK" is selected
				  {
					  homemenu = 1;  // Go back to the home menu
					  page2menu = 0;
					  backbutton = 0;
					  enter = 0;
				  }

			  }
		  }
	  }
}

