//
//  PencilPopupLayer.cpp
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/09.
//
//

#include "PencilPopupLayer.h"

PencilPopupLayer::PencilPopupLayer()
{
    
}

PencilPopupLayer::~PencilPopupLayer()
{

}

PencilPopupLayer* PencilPopupLayer::create()
{
    PencilPopupLayer* layer = new PencilPopupLayer();
    layer->init();
    layer->autorelease();
    return layer;
}

bool PencilPopupLayer::init()
{
    BasePopupLayer::init();
    sizeLayout = layoutTable->getContentSize();
    sizeListView = Size(sizeLayout.width *4/5, sizeLayout.height*2 / 3);
    
    // title
    auto title = Text::create("Choose a pencil", "fonts/keifont.ttf", 60);
    title->setPosition(Vec2(sizeLayout.width/2, sizeLayout.height - PADDING));
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setColor(Color3B::ORANGE);
    layoutTable->addChild(title);
    
    //coin image
    auto coinImg = Sprite::createWithSpriteFrameName("image_coin_64x64.png");
    coinImg->setPosition(Vec2(PADDING_MENU_HEADER_ITEM, title->getPositionY() - title->getContentSize().height/2));
    coinImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    layoutTable->addChild(coinImg);
    
    //text current coin
    curTextCoin = Text::create("", "fonts/keifont.ttf", 50);
    curTextCoin->setColor(Color3B::RED);
    curTextCoin->setString(to_string(SceneManager::getInstance()->getCurCoin()));
    curTextCoin->setPosition(Vec2(coinImg->getPositionX() + coinImg->getContentSize().width + PADDING, coinImg->getPositionY()));
    curTextCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    layoutTable->addChild(curTextCoin);
    
    // add listview horizontal
    listViewPencils = ListView::create();
    listViewPencils->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    listViewPencils->setGravity(cocos2d::ui::ListView::Gravity::CENTER_HORIZONTAL);
    listViewPencils->setTouchEnabled(true);
    listViewPencils->setBounceEnabled(true);
    listViewPencils->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    listViewPencils->setPosition(Vec2(title->getPositionX(), sizeLayout.height/2));
    listViewPencils->setContentSize(sizeListView);
    listViewPencils->setInertiaScrollEnabled(true);
    listViewPencils->setItemsMargin(LIST_SMALL_ITEM_MARGIN);
    listViewPencils->addEventListener(CC_CALLBACK_2(PencilPopupLayer::selectedItemEvent, this));
    listViewPencils->addEventListener(CC_CALLBACK_2(PencilPopupLayer::scrollEvent, this));

    layoutTable->addChild(listViewPencils);
    
    // reload data
    reloadData();
    
    // button
    Button* buyButton = Button::create("menu_btn_lang_normal.png", "", "", TextureResType::PLIST);
    buyButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    buyButton->setPosition(Vec2(sizeLayout.width/3, PADDING_MENU_HEADER_ITEM));
    buyButton->setTag(TAG_PENCIL_ITEM::BUY_BUTTON);
    buyButton->addTouchEventListener(CC_CALLBACK_2(PencilPopupLayer::touchButtonEvent, this));
    // name buy button
    Text* nameBuyButton = Text::create("Buy pencil", "fonts/keifont.ttf", 40);
    nameBuyButton->setColor(Color3B::WHITE);
    nameBuyButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    nameBuyButton->setPosition(buyButton->getContentSize()/2);
    buyButton->addChild(nameBuyButton);
    layoutTable->addChild(buyButton);
    
    Button* useButton = Button::create("menu_btn_lang_selected.png", "", "", TextureResType::PLIST); 
    useButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    useButton->setPosition(Vec2(sizeLayout.width*2/3, PADDING_MENU_HEADER_ITEM));
    useButton->setTag(TAG_PENCIL_ITEM::SELECT_BUTTON);
    useButton->addTouchEventListener(CC_CALLBACK_2(PencilPopupLayer::touchButtonEvent, this));
    layoutTable->addChild(useButton);
    // name use button
    Text* nameUseButton = Text::create("Use pencil", "fonts/keifont.ttf", 40);
    nameUseButton->setColor(Color3B::WHITE);
    nameUseButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    nameUseButton->setPosition(useButton->getContentSize()/2);
    useButton->addChild(nameUseButton);
    
    return true;
}

void PencilPopupLayer::reloadData()
{
    listViewPencils->removeAllItems();
    listPencils.clear();
    
    for (int i = 0; i < PENCIL_MAX; i++) {
        // container
        Layout* mainLayout = Layout::create();
        auto pencil = Button::create("image_pencil.png", "", "", TextureResType::PLIST);
        sizePencil = pencil->getContentSize();
    
        if ( i == 0 || i == PENCIL_MAX-1) {
            mainLayout->setContentSize(Size(sizeListView.width/2 + sizePencil.width/2, sizeListView.height));
            if (i == 0) {
                pencil->setPosition(Vec2(mainLayout->getContentSize().width - sizePencil.width/2, sizeListView.height/2));
            } else if ( i == PENCIL_MAX-1) {
                pencil->setPosition(Vec2(sizePencil.width/2, sizeListView.height/2));
            }
        } else {
            mainLayout->setContentSize(Size(pencil->getContentSize().width, sizeListView.height));
            pencil->setPosition(mainLayout->getContentSize()/2);
        }
        
        // pencil sprite
        pencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

        // pencil number
        Text* pencilNumber = Text::create(StringUtils::format("%d",i+1), "fonts/keifont.ttf", 40);
        pencilNumber->setColor(Color3B::BLACK);
        pencilNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        pencilNumber->setPosition(pencil->getContentSize()/2);
        pencil->addChild(pencilNumber);
        
        if (i >= SceneManager::getInstance()->getCurUnlockPencil()) {
            pencil->setBright(false);
            
            // add lock
            auto lock = Sprite::createWithSpriteFrameName("menu_lock_icon_64x64.png");
            lock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lock->setPosition(sizePencil/2);
            pencil->addChild(lock);
        }
        listPencils.pushBack(pencil);
        mainLayout->addChild(pencil);
        // insert listview
        listViewPencils->insertCustomItem(mainLayout, i);
    }
    listViewPencils->refreshView();
    // scroll to now using pencil
    widthLayoutPencil = -(sizePencil.width + LIST_SMALL_ITEM_MARGIN);
    this->scrollToItem(SceneManager::getInstance()->getCurUsePencil());
}

void PencilPopupLayer::selectedItemEvent(cocos2d::Ref *pSender, ListView::EventType type)
{
    ListView* list = static_cast<ListView*>(pSender);
    
    switch (type) {
        case ListView::EventType::ON_SELECTED_ITEM_START :
            break;
            
        case ListView::EventType::ON_SELECTED_ITEM_END :
        {
            this->scrollToItem((int) list->getCurSelectedIndex());
            break;
        }
            
        default:
            break;
    }
}

void PencilPopupLayer::scrollEvent(cocos2d::Ref *pSender, ui::ScrollView::EventType type)
{
    ListView* list = static_cast<ListView*>(pSender);
    
    if ( type == ui::ScrollView::EventType::SCROLLING || type == ui::ScrollView::EventType::BOUNCE_LEFT || type == ui::ScrollView::EventType::BOUNCE_RIGHT)
    {
        float curPosX = list->getInnerContainer()->getPositionX();
        auto button = (Button*)layoutTable->getChildByTag(TAG_PENCIL_ITEM::SELECT_BUTTON);
        if (curPosX < widthLayoutPencil * (SceneManager::getInstance()->getCurUnlockPencil()-1)) {
            button->setCascadeColorEnabled(false);
            button->setTouchEnabled(false);
            button->setBright(false);
        } else {
            button->setTouchEnabled(true);
            button->setBright(true);
        }
        // scale item
        this->scaleItems(curPosX);
    } else {
        this->autoFocusItemList(false);
    }
}

void PencilPopupLayer::scrollToItem(int index)
{
    float curPos = listViewPencils->getInnerContainer()->getPositionX();
    float exactPos = widthLayoutPencil * index;
    if (curPos > (exactPos + DELTA_TRANSLATE) || curPos < (exactPos - DELTA_TRANSLATE)) {
        auto moveAndScale = Sequence::create(MoveTo::create(0.3f, Vec2(exactPos, 0)), CallFunc::create(CC_CALLBACK_0(PencilPopupLayer::scrollEvent, this, listViewPencils, ui::ScrollView::EventType::SCROLLING)),NULL);
        listViewPencils->getInnerContainer()->runAction(moveAndScale);
    }
}

void PencilPopupLayer::scaleItems(float curPosX)
{

    int index = -1;
    float scale = MIN_SCALE_PENCIL;
    
    for (int i = 0 ; i < PENCIL_MAX; i++) {
        
        float startX = widthLayoutPencil * i + sizePencil.width;
        float endX = widthLayoutPencil * i - sizePencil.width;
        float centerX = (startX + endX) / 2;
        
        if (curPosX <= startX && curPosX >= endX) {
            if (curPosX > (centerX + DELTA_TRANSLATE)) {
                scale = MAX_SCALE_PENCIL - (curPosX-centerX) / sizePencil.width;
            } else if ( curPosX < (centerX - DELTA_TRANSLATE)) {
                scale = MAX_SCALE_PENCIL - (centerX - curPosX) / sizePencil.width;
            } else  if (curPosX <=(centerX+DELTA_TRANSLATE) && curPosX >=(curPosX-DELTA_TRANSLATE)){
                scale = MAX_SCALE_PENCIL;
            }
            
            index = i;
            break;
        }
    }

    if (index == -1) return;
    
    for(int i = 0; i< listPencils.size(); i++) {
        auto layout = (Layout*)listPencils.at(i);
        if (i == (index)) {
            layout->setScale(scale);
        } else {
            layout->setScale(MIN_SCALE_PENCIL);
        }
    }
}


void PencilPopupLayer::autoFocusItemList(bool isSelectedPencil)
{
    float curPosX = listViewPencils->getInnerContainer()->getPositionX();
    int index = (int)(curPosX / widthLayoutPencil);
    float _index = curPosX / widthLayoutPencil;
    if (_index - index > 0.5) {
        index++;
    }
    this->scrollToItem(index);
    
    if (isSelectedPencil) {
        if (index < SceneManager::getInstance()->getCurUnlockPencil()) {
            SceneManager::getInstance()->saveUsePencil(index);
        }
    }
}

void PencilPopupLayer::touchButtonEvent(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    auto receiver = (Button*) pSender;
    if (type == ui::Widget::TouchEventType::ENDED) {
        switch (receiver->getTag()) {
            case TAG_PENCIL_ITEM::BUY_BUTTON :
            {
                this->autoFocusItemList(true);
                break;
            }
                
            case TAG_PENCIL_ITEM::SELECT_BUTTON :
            {
                this->autoFocusItemList(true);
                break;
            }
                
            default:
                break;
        }
    }
}