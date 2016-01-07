#!/usr/bin/env python
# -*- encoding: utf-8 -*-
# vim: set et sw=4 ts=4 sts=4 ff=unix fenc=utf8:
# Author: Binux<i@binux.me>
#         http://binux.me
# Created on 2014-10-19 15:37:46

import time
import json
import logging
from six.moves import queue as Queue
logger = logging.getLogger("result")


class ResultWorker(object):

    """
    do with result
    override this if needed.
    """

    def __init__(self, resultdb, inqueue, job):
        self.resultdb = resultdb
        self.inqueue = inqueue
        self.job = job
        self._quit = False
        
    def push_job(self, task, result):
        '''51job result push redis queue'''
        if not result:
            return
        if 'taskid' in task and 'project' in task and 'url' in task:
            logger.info('result %s:%s %s -> %.30r' % (
                task['project'], task['taskid'], task['url'], result))
            result_buffer = "c_name:" + result['c_name'] + "||||" + "c_head:" + result['c_head'] + "||||" + "c_introduce:" + result['c_introduce'] + "||||" + "c_mainpage:" + result['c_mainpage'] + "||||" + "p_name:" + result['p_name'] + "||||" + "p_head:"+ result['p_head']  + "||||" + "p_body:" + result['p_body']   
            print result_buffer
            self.job.push(result_buffer)
        else:
            logger.warning('result UNKNOW -> %.30r' % result)
            return
        
    def on_result(self, task, result):
        '''Called every result'''
        if not result:
            return
        if 'taskid' in task and 'project' in task and 'url' in task:
            logger.info('result %s:%s %s -> %.30r' % (
                task['project'], task['taskid'], task['url'], result))
            ret = task['url'].find('c.html');
            if (ret != -1):
               return self.resultdb.save(
                project=task['project'],
                taskid=task['taskid'],
                url=task['url'],
                result=result
            )
            else:
                return
        else:
            logger.warning('result UNKNOW -> %.30r' % result)
            return

    def quit(self):
        self._quit = True

    def run(self):
        '''Run loop'''
        logger.info("result_worker starting...")

        while not self._quit:
            try:
                task, result = self.inqueue.get(timeout=1)
                self.push_job(task, result)
              #  self.on_result(task, result)
            except Queue.Empty as e:
                continue
            except KeyboardInterrupt:
                break
            except AssertionError as e:
                logger.error(e)
                continue
            except Exception as e:
                logger.exception(e)
                continue

        logger.info("result_worker exiting...")


class OneResultWorker(ResultWorker):
    '''Result Worker for one mode, write results to stdout'''
    def on_result(self, task, result):
        '''Called every result'''
        if not result:
            return
        if 'taskid' in task and 'project' in task and 'url' in task:
            logger.info('result %s:%s %s -> %.30r' % (
                task['project'], task['taskid'], task['url'], result))
            print(json.dumps({
                'taskid': task['taskid'],
                'project': task['project'],
                'url': task['url'],
                'result': result,
                'updatetime': time.time()
            }))
        else:
            logger.warning('result UNKNOW -> %.30r' % result)
            return
