#include <stdio.h>
#include <stdint.h>
#include <capnp/message.h>
#include <capnp/serialize.h>

#include "msg.capnp.h"

int
main(int argc, char *argv[])
{
	printf("hello world!\n");

	capnp::MallocMessageBuilder msg;
	Person::Builder person = msg.initRoot<Person>();
	person.setId(123);
	person.setName("eli");

	// this has to be two lines or words will be released.
	auto words = capnp::messageToFlatArray(msg);
	auto bytes = words.asBytes();
	uint8_t *ptr = bytes.begin();
	size_t sz = bytes.size();
	printf("serialized to array length %zu at %p\n", sz, ptr);

	// simulate sending the bytes in ptr over the network and memcpying into words2
	auto words2 = kj::heapArray<capnp::word>(sz/sizeof(capnp::word));
	memcpy(words2.begin(), ptr, sz);
	capnp::FlatArrayMessageReader msg2(words2);
	Person::Reader person2 = msg2.getRoot<Person>();
	printf("got id: %" PRIu32 ", name %s\n", person2.getId(), person2.getName().cStr());

	return 0;
}