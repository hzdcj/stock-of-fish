#ifndef _CursorTextField_H_
#define _CursorTextField_H_

#include "cocos2d.h"

USING_NS_CC;

class CursorTextField : public TextFieldTTF, public TextFieldDelegate, public IMEDelegate
{
private:

	Point m_beginPos;
	Sprite * m_pCursorSprite;
	Action *m_pCursorAction;
	Point m_cursorPos;

	float inputFrameWidth;

	float inputMaxLength;
	int nLenCount;
	int codeNumType[50];    //每个字符对应的字节数量
	int codeCur;         //当前第几个字符
	int startCur;        //行开头字符下标
	int endCur;          //行末尾下标
	// 输入框总内容
	std::string m_pInputText;
	std::string inpuText; //当前输入框内容
public:
	CursorTextField();
	~CursorTextField();

	static CursorTextField * textFieldWithPlaceHolder(cocos2d::Node * node, const char *placeholder, const char *fontName, float fontSize);

	void onEnter();
	void onExit();
	bool init();
	
	void initCursorSprite(int nHeight);


	virtual bool onTextFieldAttachWithIME(TextFieldTTF *pSender) override;
	virtual bool onTextFieldDetachWithIME(TextFieldTTF * pSender) override;
	virtual bool onTextFieldInsertText(cocos2d::TextFieldTTF*  sender, const char * text, size_t nLen) override;
	virtual bool onTextFieldDeleteBackward(cocos2d::TextFieldTTF*  sender, const char * delText, size_t nLen) override;



	virtual void setPosition(const Point& pos);
	virtual void setPosition(float &x, float &y);

	void setCursorPositionX(float x); // 设置光标x位置
	// 把光标添加到和输入框一起的层中
	void AddCursor(Node *node);
	// Layer Touch
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);

	// 判断是否点击在TextField处
	bool isInTextField(Touch *pTouch);
	// 得到TextField矩形
	Rect getRect();

	// 打开输入法
	void openIME();
	// 关闭输入法
	void closeIME();

	std::string split_text(std::string name, int len, int start);

	const char* getInputText();
	void setInpuntText(char* text);
	void setInputWidth(float width);
	void setInputMaxLength(float length);


	cocos2d::Node * parentNode;



protected:
	EventListenerTouchOneByOne * listener;
};

#endif
