#include "GBAApp.h"

#include "GameController.h"

#include <QFileOpenEvent>

using namespace QGBA;

GBAApp::GBAApp(int& argc, char* argv[])
	: QApplication(argc, argv)
{
    QApplication::setApplicationName(PROJECT_NAME);
    QApplication::setApplicationVersion(PROJECT_VERSION);

	if (parseArguments(&m_args, &m_opts, argc, argv, 0)) {
		m_window.setOptions(&m_opts);
		m_window.argumentsPassed(&m_args);
	} else {
		m_window.setOptions(&m_opts);
	}

    m_window.show();
}

GBAApp::~GBAApp() {
	freeArguments(&m_args);
	GBAConfigFreeOpts(&m_opts);
}

bool GBAApp::event(QEvent* event) {
	if (event->type() == QEvent::FileOpen) {
		m_window.controller()->loadGame(static_cast<QFileOpenEvent*>(event)->file());
		return true;
	}
	return QApplication::event(event);
}