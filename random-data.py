from dcd.entities.thing import Thing
from dcd.entities.property import PropertyType

from dotenv import load_dotenv
import os
# The thing ID and access token
load_dotenv()
THING_ID = os.environ['THING_ID']
THING_TOKEN = os.environ['THING_TOKEN']

# Instantiate a thing with its credential
my_thing = Thing(thing_id=THING_ID, token=THING_TOKEN)

# We can fetch the details of our thing
my_thing.read()

print(my_thing.to_json())

# If we have no properties, let's create a random one
my_property = my_thing.find_or_create_property("My Random Property",
                                               PropertyType.THREE_DIMENSIONS)

# Let's have a look at the property, it should
# contains the name, a unique id and the dimensions
print(my_property.to_json())
