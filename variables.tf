variable "dnsimple_account" {}
variable "dnsimple_token" {}

variable "domain" {
  default = "gethaterblock.com"
}

variable "bucket" {
  default = "haterblock-client"
}

variable "regions" {
  type = "map"

  default = {
    "eu" = "eu-west-1"
    "us" = "us-east-1"
  }
}
