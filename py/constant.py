# used by bash and python

CRAWL_DIR="/mnt/meitian-rsync/data"

# input redis queue for online_clustering
CLUSTER_QUEUE_NAME="clustering_engine"
CLUSTER_QUEUE_HOST="cache1.yuezhixing.cn"
CLUSTER_QUEUE_PORT="6379"
CLUSTER_QUEUE_PASSWD="ce4fa10ee37d40f7"

# redis server for storing docInfo
REDIS_HOST="cache1.yuezhixing.cn"
REDIS_PORT="6379"
REDIS_PASSWD="ce4fa10ee37d40f7"

# page server
PAGE_HOST="127.0.0.1"
PAGE_PORT="8767"

# bs
BS_HOST="127.0.0.1"
BS_PORT="19002"

# max similar docs
MAX_SIMILAR_DOCS=10
KEY_EXPIRED_SECONDS=604800
