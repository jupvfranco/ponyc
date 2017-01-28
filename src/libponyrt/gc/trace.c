#include "trace.h"
#include "gc.h"
#include "../sched/scheduler.h"
#include "../sched/cpu.h"
#include "../actor/actor.h"
#include <assert.h>
#include <dtrace.h>

void pony_gc_send(pony_ctx_t* ctx)
{
#ifdef NOGC 
  // nothing
#else
  assert(ctx->stack == NULL);
  ctx->trace_object = ponyint_gc_sendobject;
  ctx->trace_actor = ponyint_gc_sendactor;

  #ifdef USE_TELEMETRY
    ctx->tsc = ponyint_cpu_tick();
  #endif
  DTRACE1(GC_SEND_START, (uintptr_t)ctx->scheduler);
#endif
}

void pony_gc_recv(pony_ctx_t* ctx)
{
#ifdef NOGC 
  // nothing
#else
  assert(ctx->stack == NULL);
  ctx->trace_object = ponyint_gc_recvobject;
  ctx->trace_actor = ponyint_gc_recvactor;

  #ifdef USE_TELEMETRY
    ctx->tsc = ponyint_cpu_tick();
  #endif
  DTRACE1(GC_RECV_START, (uintptr_t)ctx->scheduler);
#endif
}

void ponyint_gc_mark(pony_ctx_t* ctx)
{
  assert(ctx->stack == NULL);
  ctx->trace_object = ponyint_gc_markobject;
  ctx->trace_actor = ponyint_gc_markactor;
}

void pony_gc_acquire(pony_ctx_t* ctx)
{
  assert(ctx->stack == NULL);
  ctx->trace_object = ponyint_gc_acquireobject;
  ctx->trace_actor = ponyint_gc_acquireactor;
}

void pony_gc_release(pony_ctx_t* ctx)
{
  assert(ctx->stack == NULL);
  ctx->trace_object = ponyint_gc_releaseobject;
  ctx->trace_actor = ponyint_gc_releaseactor;
}

void pony_send_done(pony_ctx_t* ctx)
{
#ifdef NOGC 
  // nothing
#else
  ponyint_gc_handlestack(ctx);
  ponyint_gc_sendacquire(ctx);
  ponyint_gc_done(ponyint_actor_gc(ctx->current));

  #ifdef USE_TELEMETRY
    ctx->time_in_send_scan += (ponyint_cpu_tick() - ctx->tsc);
  #endif
  DTRACE1(GC_SEND_END, (uintptr_t)ctx->scheduler);
#endif  
}

void pony_recv_done(pony_ctx_t* ctx)
{
#ifdef NOGC 
  // nothing
#else
  ponyint_gc_handlestack(ctx);
  ponyint_gc_done(ponyint_actor_gc(ctx->current));

  #ifdef USE_TELEMETRY
    ctx->time_in_recv_scan += (ponyint_cpu_tick() - ctx->tsc);
  #endif
  DTRACE1(GC_RECV_END, (uintptr_t)ctx->scheduler);
#endif  
}

void ponyint_mark_done(pony_ctx_t* ctx)
{
  ponyint_gc_markimmutable(ctx, ponyint_actor_gc(ctx->current));
  ponyint_gc_handlestack(ctx);
  ponyint_gc_sendacquire(ctx);
  ponyint_gc_sweep(ctx, ponyint_actor_gc(ctx->current));
  ponyint_gc_done(ponyint_actor_gc(ctx->current));
}

void pony_acquire_done(pony_ctx_t* ctx)
{
  ponyint_gc_handlestack(ctx);
  ponyint_gc_sendacquire(ctx);
  ponyint_gc_done(ponyint_actor_gc(ctx->current));
}

void pony_release_done(pony_ctx_t* ctx)
{
  ponyint_gc_handlestack(ctx);
  ponyint_gc_sendrelease_manual(ctx);
  ponyint_gc_done(ponyint_actor_gc(ctx->current));
}

void pony_send_next(pony_ctx_t* ctx)
{
  ponyint_gc_handlestack(ctx);
  ponyint_gc_done(ponyint_actor_gc(ctx->current));
}

void pony_trace(pony_ctx_t* ctx, void* p)
{
#ifdef NOGC
  // nothing 
#else    
  ctx->trace_object(ctx, p, NULL, PONY_TRACE_OPAQUE);
#endif
}

void pony_traceknown(pony_ctx_t* ctx, void* p, pony_type_t* t, int m)
{
#ifdef NOGC
  // nothing 
#else  
  if(t->dispatch != NULL)
  {
    ctx->trace_actor(ctx, (pony_actor_t*)p);
  } else {
    ctx->trace_object(ctx, p, t, m);
  }
#endif
}

void pony_traceunknown(pony_ctx_t* ctx, void* p, int m)
{
#ifdef NOGC
  // nothing 
#else 
  pony_type_t* t = *(pony_type_t**)p;

  if(t->dispatch != NULL)
  {
    ctx->trace_actor(ctx, (pony_actor_t*)p);
  } else {
    ctx->trace_object(ctx, p, t, m);
  }
#endif
}
