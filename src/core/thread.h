/* Copyright (c) 2013-2016 Jeffrey Pfau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef M_CORE_THREAD_H
#define M_CORE_THREAD_H

#include "util/common.h"

#include "core/log.h"
#include "core/rewind.h"
#include "core/sync.h"
#include "util/threading.h"

struct mCoreThread;
struct mCore;

typedef void (*ThreadCallback)(struct mCoreThread* threadContext);

enum mCoreThreadState {
	THREAD_INITIALIZED = -1,
	THREAD_RUNNING = 0,
	THREAD_REWINDING,
	THREAD_MAX_RUNNING = THREAD_REWINDING,
	THREAD_INTERRUPTED,
	THREAD_INTERRUPTING,
	THREAD_PAUSED,
	THREAD_PAUSING,
	THREAD_RUN_ON,
	THREAD_WAITING,
	THREAD_RESETING,
	THREAD_EXITING,
	THREAD_SHUTDOWN,
	THREAD_CRASHED
};

struct mCoreThread;
struct mThreadLogger {
	struct mLogger d;
	struct mCoreThread* p;
};

struct mCoreThread {
	// Input
	struct mCore* core;

	// Threading state
	Thread thread;
	enum mCoreThreadState state;

	Mutex stateMutex;
	Condition stateCond;
	enum mCoreThreadState savedState;
	int interruptDepth;
	bool frameWasOn;

	struct mThreadLogger logger;
	enum mLogLevel logLevel;
	ThreadCallback startCallback;
	ThreadCallback resetCallback;
	ThreadCallback cleanCallback;
	ThreadCallback frameCallback;
	void* userData;
	void (*run)(struct mCoreThread*);

	struct mCoreSync sync;
	struct mCoreRewindContext rewind;
};

bool mCoreThreadStart(struct mCoreThread* threadContext);
bool mCoreThreadHasStarted(struct mCoreThread* threadContext);
bool mCoreThreadHasExited(struct mCoreThread* threadContext);
bool mCoreThreadHasCrashed(struct mCoreThread* threadContext);
void mCoreThreadMarkCrashed(struct mCoreThread* threadContext);
void mCoreThreadEnd(struct mCoreThread* threadContext);
void mCoreThreadReset(struct mCoreThread* threadContext);
void mCoreThreadJoin(struct mCoreThread* threadContext);

bool mCoreThreadIsActive(struct mCoreThread* threadContext);
void mCoreThreadInterrupt(struct mCoreThread* threadContext);
void mCoreThreadInterruptFromThread(struct mCoreThread* threadContext);
void mCoreThreadContinue(struct mCoreThread* threadContext);

void mCoreThreadRunFunction(struct mCoreThread* threadContext, void (*run)(struct mCoreThread*));

void mCoreThreadPause(struct mCoreThread* threadContext);
void mCoreThreadUnpause(struct mCoreThread* threadContext);
bool mCoreThreadIsPaused(struct mCoreThread* threadContext);
void mCoreThreadTogglePause(struct mCoreThread* threadContext);
void mCoreThreadPauseFromThread(struct mCoreThread* threadContext);
void mCoreThreadWaitFromThread(struct mCoreThread* threadContext);
void mCoreThreadStopWaiting(struct mCoreThread* threadContext);

void mCoreThreadSetRewinding(struct mCoreThread* threadContext, bool);

struct mCoreThread* mCoreThreadGet(void);

void mCoreThreadFrameStarted(struct mCoreThread*);
void mCoreThreadFrameEnded(struct mCoreThread*);

struct mLogger* mCoreThreadLogger(void);

#endif
