#include "ScoresScene.h"
#include "ScenesManager.h"
#include "Common\sqlite3.h"

ScoresScene *ScoresScene::scoresScene = nullptr;
const std::string ScoresScene::pathToBack = "background.png";
const std::string ScoresScene::pathToBackEditBox = "gray_back.png";
const std::string ScoresScene::labelHightScoresName = "HightScores";
std::vector<std::tuple<int, bool, std::string, int> > ScoresScene::dbRows;
std::vector<std::tuple<int, bool, std::string, int> > ScoresScene::dbRowsPreparedForDeletion;

ScoresScene::~ScoresScene() {
	dbRows.clear();
	MessagesQueue::getInstance()->removeListener(MessagesQueue::MessageType::CREATE_SCORES_TABLE, *_createTableCallback);
	CC_SAFE_DELETE(_createTableCallback);
}

Scene* ScoresScene::createScene(int scores, bool isGameOver) {
	auto scene = Scene::create();
	auto layer = ScoresScene::create();
	scoresScene = layer;
	layer->_tmpScores = scores;
	layer->_isGameOver = isGameOver;
	scene->addChild(layer);

	layer->_createTableCallback = new MessagesQueue::WrapperMessageQueueCallback_1(CC_CALLBACK_1(ScoresScene::createScoresTableCallback
			, layer), "ScoresSceneCreateTable");
	MessagesQueue::getInstance()->addListener(MessagesQueue::MessageType::CREATE_SCORES_TABLE, *layer->_createTableCallback);

	sqlite3 *db = nullptr;
	try {
		std::string pathDb = FileUtils::getInstance()->getWritablePath() + "dc.db3";
		int resultCode = sqlite3_open(pathDb.c_str(), &db);
		char *errMsg = nullptr;

		if (resultCode != SQLITE_OK)
			return nullptr;

		std::string query = "CREATE TABLE IF NOT EXISTS scores (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name TEXT NOT NULL, scores INTEGER NOT NULL)";
		resultCode = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg);
		if (resultCode != SQLITE_OK)
			return nullptr;

		query = "SELECT * FROM scores;";
		resultCode = sqlite3_exec(db, query.c_str(), selectDbCallback, nullptr, &errMsg);
		if (resultCode != SQLITE_OK)
			return nullptr;
		if (isGameOver) {
			layer->_editBox = ui::EditBox::create({ 175, 36 }, ui::Scale9Sprite::create(pathToBackEditBox));
			layer->_editBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			layer->_editBox->setPosition(Size(layer->getContentSize().width / 2, layer->getContentSize().height / 2));
			layer->_editBox->setFontName("Fixedsys");
			layer->_editBox->setFontSize(35);
			layer->_editBox->setFontColor(Color3B::WHITE);
			layer->_editBox->setPlaceHolder("Input Name");
			layer->_editBox->setPlaceholderFontColor(Color3B::GRAY);
			layer->_editBox->setMaxLength(10);
			layer->_editBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
			layer->_editBox->setDelegate(&layer->_editBoxDelegate);
			layer->addChild(layer->_editBox);
		}
		else {
			MessagesQueue::Message msg = { MessagesQueue::MessageType::CREATE_SCORES_TABLE, nullptr };
			MessagesQueue::getInstance()->addMessageToQueue(msg);
		}

		sqlite3_close(db);
	}
	catch (...) {
		sqlite3_close(db);
		throw;
	}
	return scene;
}

void ScoresScene::ScoresSceneDelegate::editBoxReturn(ui::EditBox* editBox) {
	std::string name = editBox->getText();
	dbRows.push_back(std::make_tuple(-1, true, name, scoresScene->_tmpScores));

	ScoresScene::getInstance()->sortAndDeleteDbRows();
	ScoresScene::getInstance()->removeChild(editBox, true);
	MessagesQueue::Message msg = { MessagesQueue::MessageType::CREATE_SCORES_TABLE, nullptr };
	MessagesQueue::getInstance()->addMessageToQueue(msg);
}

void ScoresScene::sortAndDeleteDbRows() {
	const int limit = 17;
	std::sort(dbRows.begin(), dbRows.end(), [](std::tuple<int, bool, std::string, int> tupleA
		, std::tuple<int, bool, std::string, int> tupleB)->bool {
		int a = std::get<3>(tupleA);
		int b = std::get<3>(tupleB);
		if (a <= b)
			return false;
		else
			return true;
	});

	if (dbRows.size() > limit) {
		dbRowsPreparedForDeletion.assign(dbRows.begin() + limit, dbRows.end());
		dbRows.erase(dbRows.begin() + limit, dbRows.end());
		sqlite3 *db = nullptr;
		try {
			std::string pathDb = FileUtils::getInstance()->getWritablePath() + "dc.db3";
			int resultCode = sqlite3_open(pathDb.c_str(), &db);
			char *errMsg = nullptr;
			char query[1024];

			for (auto tuple : dbRowsPreparedForDeletion) {
				sprintf(query, "DELETE FROM scores WHERE id = %d", std::get<0>(tuple));
				resultCode = sqlite3_exec(db, query, nullptr, nullptr, &errMsg);
				CC_ASSERT(resultCode == SQLITE_OK);
			}
			dbRowsPreparedForDeletion.clear();
			sqlite3_close(db);
		}
		catch (...) {
			sqlite3_close(db);
			throw;
		}
	}
}

void ScoresScene::createScoresTableCallback(void*) {
	const int padding = 36;
	Label *labelScores = static_cast<Label*>(getChildByName(labelHightScoresName));
	Size size = getContentSize();
	Label *labelUserScore = nullptr;
	std::string userName;
	int counter = 1;

	sortAndDeleteDbRows();

	for (auto tuple : dbRows) {
		userName = std::get<2>(tuple);
		userName += " - ";
		userName += std::to_string(std::get<3>(tuple));
		labelUserScore = Label::createWithSystemFont(userName, "Fixedsys", 35);
		labelUserScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelUserScore->setPosition({ size.width / 2, labelScores->getPositionY() - (padding * counter) });
		counter++;
		addChild(labelUserScore);
	}
	if (dbRows.size() && _isGameOver)
		writeScoresToDb();
}

void ScoresScene::writeScoresToDb() {
	sqlite3 *db = nullptr;
	try {
		std::string pathDb = FileUtils::getInstance()->getWritablePath() + "dc.db3";
		int resultCode = sqlite3_open(pathDb.c_str(), &db);
		char *errMsg = nullptr;

		CC_ASSERT(resultCode == SQLITE_OK);

		char query[1024];
		
		for (auto tuple : dbRows) {
			bool isInsertNeeded = std::get<1>(tuple);
			if (isInsertNeeded) {
				sprintf(query, "INSERT INTO scores(name, scores) VALUES('%s', %d)", std::get<2>(tuple).c_str(), std::get<3>(tuple));
				resultCode = sqlite3_exec(db, query, nullptr, nullptr, &errMsg);
				CC_ASSERT(resultCode == SQLITE_OK);
			}
		}

		dbRows.clear();
		sqlite3_close(db);
	}
	catch (...) {
		sqlite3_close(db);
		throw;
	}
}

/*
* typedef int (*sqlite3_callback)(
* void*,    Data provided in the 4th argument of sqlite3_exec() 
* int,      The number of columns in row
* char**,   An array of strings representing fields in the row
* char**    An array of strings representing column names
* );
*/
int ScoresScene::selectDbCallback(void *data, int columnsCount, char **fields, char **columnNames) {
	dbRows.push_back(std::make_tuple(std::atoi(fields[0]), false, std::string(fields[1]), std::atoi(fields[2])));
	return SQLITE_OK;
}

bool ScoresScene::init() {
	if (!Layer::init())
		return false;

	const int fraction = 16;
	Size size = getContentSize();
	auto background = Sprite::create(pathToBack);
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	background->setScaleX(size.width / background->getContentSize().width);
	background->setScaleY(size.height / background->getContentSize().height);
	addChild(background, -1);
	
	auto label = Label::createWithSystemFont("HIGHT SCORES", "Fixedsys", 45);
	label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	label->setPosition({ size.width / 2, size.height - (size.height / fraction) });
	label->enableShadow();
	label->setName(labelHightScoresName);
	addChild(label, 0);

	auto labelReturn = Label::createWithSystemFont("Return", "Fixedsys", 45);
	label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	auto menuItemReturn = MenuItemLabel::create(labelReturn
		, CC_CALLBACK_1(ScenesManager::buttonReturnOptionsCallback, ScenesManager::getInstance()));
	auto menu = Menu::createWithItem(menuItemReturn);
	menu->setPosition({ size.width / 2, size.height / fraction });
	addChild(menu, 0);

	return true;
}