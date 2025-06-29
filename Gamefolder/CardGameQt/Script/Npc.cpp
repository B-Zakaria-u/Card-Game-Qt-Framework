#include "Npc.h"
#include <QFile>
#include <QRandomGenerator>
#include <QStandardPaths>

npc::npc(){}
npc::npc (Deckk* deckk,field* Fieldd,int Diff) : deck(deckk) ,Field(Fieldd), Difficulty_Tier(Diff) {
    score = 0;
    Turn = false;
}
npc::~npc () {};
bool npc::setTurn () {
    if(Turn){
        Turn = false;
        qDebug() << " tpassa tour dial bot  "<<Turn;
        return Turn;
    } else {
        Turn =true;
        qDebug() << " tour dial lbot "<<Turn;
        return Turn;
    }
}
int npc::getCurrentScore () {
    return score;
}
int npc::Updatescore (bool Win) {
    if(Win){
        return score++;
    } else {
        return -1;
    }
}
QVBoxLayout* npc::ShowCardImg () {
    QString GameFolderPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString CurrentcardImg = QString( GameFolderPath + "/Gamefolder/CardGameQt/Assets/Cards/BackCard.png");
    QFile file(CurrentcardImg);
    if (!file.exists()) {
        qDebug() << "File not found: " << CurrentcardImg;
    }
    //qDebug() << "File found : " << CurrentcardImg;
    QImage* CardImg = new QImage(CurrentcardImg);
    //qDebug() << "I append the img ";
    qDebug() <<CardImg->size();
    CardLabel = new QLabel;
    //qDebug() << "I append to it the label ";
    CardLabel->setPixmap(QPixmap::fromImage(CardImg->scaledToHeight(150)));
    //qDebug() << "I fix its size";
    CardLabel->setFixedSize(120, 170);
    CardLabel->setAlignment(Qt::AlignCenter); // To center the image with the hitbox, u ccan take of the Stylesheet to test it
    CardLabel->setStyleSheet("background-color: transparent;");
    QVBoxLayout* Cardzone = new QVBoxLayout;
    //qDebug() << "I fix its boxlayout ";
    Cardzone->addWidget(CardLabel);
    //qDebug() << "I create the complete ";
    qDebug() <<Cardzone->sizeHint();
    return Cardzone;
}
bool npc::getTurn () {
    return Turn;
}
QList<int> npc::getHandCards (){
    return CardsInHand;
}
int npc::getHandCardsize (){
    return CardsInHand.size();
}

int npc::getFirstCard () {
    return this->CardsInHand.takeFirst();
}
void npc::DropCard () {
    int Cardindex = CardsInHand.indexOf(SelectedCard);
    qDebug() << " I will drop the card (NPC): "<<SelectedCard;
    qDebug () <<"The size of card before deleting is (NPC):"<< CardsInHand.size();
    if (getSelectedCard()) {
        qDebug () <<"The card index is (NPC):"<< Cardindex;
        CardsInHand.remove(Cardindex);
        Field->TakeCard(SelectedCard);
        qDebug() << " I still have (NPC)" << CardsInHand.size() << " cards in my hand";
        qDebug () << "(NPC) I have in hand : "<<getHandCards();
    } else {
        qDebug() << "My hand is empty now , I have no cards(NPC)";
    }
}
QVBoxLayout* npc::Drawcard () {
    int cardnumber;
    if (this->Difficulty_Tier == 3){ //will Randomly pick a special card when hes gonna pick a card
        if((QRandomGenerator::global()->bounded(1, 4)) == 3){
            cardnumber = deck->drawSpecialcard();
        } else{
            cardnumber = deck->drawcard();
        }
    } else {
        cardnumber = deck->drawcard();
    }
    qDebug () << deck->getDecksize();
    if (!deck->isEmpty()){
        qDebug()<<"the npc picked picked :"<<cardnumber;
        CardsInHand.push_back(cardnumber);
        QVBoxLayout* card = ShowCardImg();
        return card;
    }
    qDebug()<< " i passed here to draw npc card but couldnt";
    return nullptr;
}

bool npc::VerifyCards (){
    qDebug() << "(NPC)Current Card Suit inside the field :"<<Field->getcardsuit();
    qDebug () << "(NPC)Field current card : "<<Field->getCurrentCard();
    int FieldCurrentSuit = Field->getcardsuit();
    int FieldCurrentCard = Field->getcardrank();
    int indexofCardtoplay = -1;
    int card;
    int Trying_toplay;// var that wil be used in the easy difficulty alone
    if(this->Difficulty_Tier == 3){
        qDebug () << " GL its hard mode :D";
        for(auto It = CardsInHand.begin();It != CardsInHand.end() ; It++){
            int cardrank = *It%10;
            if((cardrank == 1 || cardrank == 2) && (cardrank-1)/10 == FieldCurrentSuit) {
                indexofCardtoplay = CardsInHand.indexOf(*It);
                qDebug () << cardrank;
                qDebug () << "(NPC)Card exists with the index : "<< indexofCardtoplay;
                break;
            } else if ((*It-1)/10 == FieldCurrentSuit || *It%10 == FieldCurrentCard){
                indexofCardtoplay = CardsInHand.indexOf(*It);
                qDebug () << cardrank;
                qDebug () << "(NPC)Card exists with the index : "<< indexofCardtoplay;
                break;
            }

            }
    } else {
        for (auto start =CardsInHand.begin ();start != CardsInHand.end(); ++start){
            card = *start;
            if (this->Difficulty_Tier == 1){
                if ((card-1)/10 == FieldCurrentSuit || card%10 == FieldCurrentCard){
                    qDebug () << "Beginner trying to play ... ";
                    Trying_toplay = QRandomGenerator::global()->bounded(1, 4); // 1 chance out of 2 that the npc will use the indexed card
                    qDebug () << Trying_toplay;
                    if (Trying_toplay == 2 || CardsInHand.size() >10){ // to avoid having more than 10cards in hand
                        indexofCardtoplay = CardsInHand.indexOf(card);
                        break;
                    } else {
                        qDebug () << "I wont play a card yet ... :c";
                    }
                }
            } else if ( this->Difficulty_Tier == 2){
                //Just a Basic turn here, he check the first card alone
                if ((card-1)/10 == FieldCurrentSuit || card%10 == FieldCurrentCard){
                    indexofCardtoplay = CardsInHand.indexOf(card);
                    qDebug () << card;
                    qDebug () << "(NPC)Card exists with the index : "<< indexofCardtoplay;
                    break;
                }
            } else {
                qDebug () << "Ser ... No Difficulty yet ... Just a Safety else, ty.";
            }
        }
    }
    qDebug () << CardsInHand;
    if(indexofCardtoplay != -1){
        SelectedCard = CardsInHand.at(indexofCardtoplay);
        qDebug () << "(NPC)i have this card";
        return true;
    } else {
        qDebug () << "(NPC)i dont have a card of this type";
        return false;
    }
}
bool npc::VerifyCards (int cardd){
    qDebug () << " New Suit = "<<cardd;
    for (auto start =CardsInHand.begin ();start != CardsInHand.end(); ++start){
        if (((*start-1)/10) == cardd || *start%10 == 7){
            indexofCardtoplay = CardsInHand.indexOf(*start);
            qDebug () << *start;
            qDebug () << "(NPC)Card exists with the index : "<< indexofCardtoplay;
            break;
        }
        if (indexofCardtoplay){
            indexofCardtoplay = -1;
        }
    }
    qDebug () << CardsInHand;
    if(indexofCardtoplay != -1){
        SelectedCard = CardsInHand.at(indexofCardtoplay);
        qDebug () << "(NPC) i have this card";
        return true;
    } else {
        qDebug () << "(NPC) i dont have a card of this type";
        return false;
    }
}
int npc::Choosingthenewsuit (){
    HigherCardSuitsnbr = 0;
    int loop =0;
    while (loop < 4){
        int Highestsuit = 0;
        for (auto start =CardsInHand.begin ();start != CardsInHand.end(); ++start){
            if((*start-1) /10 == loop){
                Highestsuit ++;
            }
        }
        if (HigherCardSuitsnbr < Highestsuit){
            HigherCardSuitsnbr = loop;
            qDebug () << " I have a lot  of card with the suit :" << HigherCardSuitsnbr;
        }
        loop++;
    }
    switch(HigherCardSuitsnbr){
    case 0 : return 0;break;
    case 1 : return 1;break;
    case 2 : return 2;break;
    case 3 : return 3;break;
    default: qDebug () << " Value have a problem check it."; return -1;
    }
    //This function might be called ChoosingSuit but will be used for hardest difficulty so that the bot will make sense while playing
}
int npc::getSelectedCard () {
    if (SelectedCard){
        return SelectedCard;
    } else {
        qDebug() << "(NPC)card doesnt exist yet ";
        return 0;
    }
}
int npc::PlayingCard(){
    return SelectedCard;
}
bool npc::isEmpty () {
    return this->CardsInHand.empty();
}
