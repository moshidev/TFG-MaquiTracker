/**
 * Daniel Pedrosa Montes © 2024
 * 
 * GNU AFFERO GENERAL PUBLIC LICENSE
 */

#ifndef MB85RS4MT_TEST_H
#define MB85RS4MT_TEST_H

void MB85RS4MT_test_push_and_pull_status_register(MB85RS4MT_t* fram);

void MB85RS4MT_test_block_protect_err(MB85RS4MT_t* fram);

void MB85RS4MT_test_write_and_read_all_bytes(MB85RS4MT_t* fram);

void MB85RS4MT_test_pull_ID(MB85RS4MT_t* fram);

#endif /* MB85RS4MT_TEST_H */
