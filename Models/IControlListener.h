#pragma once

// TEST
interface IControlListener
{
	virtual void OnMouseLDown(int x, int y)=0;
	virtual void OnMouseLUp(int x, int y)=0;
	virtual void OnMouseRDown(int x, int y)=0;
	virtual void OnMouseRUp(int x, int y)=0;
	virtual void OnKeyDown(char key)=0;
	virtual void OnKeyUp(char key)=0;
};