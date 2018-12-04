#include "sched.h"

/* Global fcfs quantum parameter */
int fcfs_quantum=3;

/* Structure to store fcfs thread-specific fields */
// struct fcfs_data {
// 	int remaining_ticks_slice;
// };

// static int task_new_fcfs(task_t* t)
// {
// 	struct fcfs_data* cs_data=malloc(sizeof(struct fcfs_data));
//
// 	if (!cs_data)
// 		return 1;  /* Cannot reserve memory */
//
// 	/* initialize the quantum */
// 	cs_data->remaining_ticks_slice=fcfs_quantum;
// 	t->tcs_data=cs_data;
// 	return 0;
// }

// static void task_free_fcfs(task_t* t)
// {
// 	if (t->tcs_data) {
// 		free(t->tcs_data);
// 		t->tcs_data=NULL;
// 	}
// }

static task_t* pick_next_task_fcfs(runqueue_t* rq)
{
	task_t* t=head_slist(&rq->tasks);

	/* Cufcfsent is not on the rq -> let's remove it */
	if (t)
		remove_slist(&rq->tasks,t);

	return t;
}

static void enqueue_task_fcfs(task_t* t,runqueue_t* rq, int preempted)
{
	// struct fcfs_data* cs_data=(struct fcfs_data*) t->tcs_data;

	if (t->on_rq || is_idle_task(t))
		return;

	insert_slist(&rq->tasks,t); //Push task
	// cs_data->remaining_ticks_slice=fcfs_quantum; // Reset slice
}

// static void task_tick_fcfs(runqueue_t* rq)
// {
// 	task_t* cufcfsent=rq->cur_task;
// 	struct fcfs_data* cs_data=(struct fcfs_data*) cufcfsent->tcs_data;
//
// 	if (is_idle_task(cufcfsent))
// 		return;
//
// 	cs_data->remaining_ticks_slice--; /* Charge tick */
//
// 	if (cs_data->remaining_ticks_slice<=0)
// 		rq->need_resched=TRUE; //Force a resched !!
// }

static task_t* steal_task_fcfs(runqueue_t* rq)
{
	task_t* t=tail_slist(&rq->tasks);

	if (t)
		remove_slist(&rq->tasks,t);

	return t;
}

sched_class_t fcfs_sched= {
	// .task_new=task_new_fcfs,
	// .task_free=task_free_fcfs,
	.pick_next_task=pick_next_task_fcfs,
	.enqueue_task=enqueue_task_fcfs,
	// .task_tick=task_tick_fcfs,
	.steal_task=steal_task_fcfs
};
